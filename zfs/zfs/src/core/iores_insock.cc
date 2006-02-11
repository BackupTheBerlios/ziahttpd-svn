//
// iores_insock.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Jan 24 21:08:13 2006 texane
// Last update Sat Feb 11 18:08:46 2006 
//


#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_debug.hh>
#include <core/ziafs_status.hh>


using std::string;
using namespace status;


io::res_insock::res_insock(stmask openmod, const string& local_addr, unsigned short local_port) : resource()
{
  // This constructor creates a new local
  // connection, that's one that's bound
  // to a local address.

  // openmod should be ST_FETCH, since we
  // only want to perform accept() operation
  // on the socket descriptor

  // The socket descriptor should be set to
  // SOCKET_ERROR like code

  m_tm_lastio = 0;
  m_accepting = true;
  m_my_addr = local_addr;
  m_my_port = local_port;
}


// a new connection have been accepted
io::res_insock::res_insock(stmask openmod, const struct sockaddr_in& foreign_addr, const sysapi::insock::handle_t& hsock) : resource()
{
  // This constructor creates a socket
  // from the descriptor returned by the
  // accept() function.

  // This resource should be opened
  // in both ST_FETCHING and ST_FEEDING
  // modes.

  m_tm_lastio = 0;
  m_accepting = false;
  m_hsock = hsock;
  m_foreign_addr = foreign_addr;
}


io::res_insock::~res_insock()
{
  // Nothing to be done here,
  // actual close function is
  // done by the resource manager.
}


status::error io::res_insock::io_on_open()
{
  // This function performs the actual
  // resource opening.

  // In this function is the first bottle neck.
  // Actually, launching a test with threads sleeping
  // during 1 second, then disconnecting cause the backlog
  // queue to be flooded.
  // Solvable by adding a thread to handle incoming connectioon

  if (m_accepting == true)
    {
      if (sysapi::insock::p_to_inaddr(m_local_addr, m_my_addr, m_my_port) != sysapi::error::SUCCESS)
	ziafs_return_status( CANNOT_OPEN );
      if (sysapi::insock::create_listening(m_hsock, m_local_addr, SOMAXCONN) != sysapi::error::SUCCESS)
	ziafs_return_status( CANNOT_OPEN );
    }

  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::io_on_close(void*& aux)
{
  net::server* srv;

  srv = (net::server*)aux;

  ++srv->m_resman->m_nr_connections;
  sysapi::insock::close(m_hsock);
  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::io_on_read(void*& pdata, void*& aux)
{
  net::server* srv;
  net::session* sess_client;
  net::protocol* proto;
  sysapi::insock::handle_t hsock;
  sysapi::error::handle_t err;
  resource* res;
  stmask omode;
  int addrlen;
  unsigned int nread;
  bool done;

  srv = (net::server*&)aux;

  // This is an accepting socket,
  // reading means accetping an
  // incoming connection.
  if (m_accepting == true)
    {
      // Create the new resource
      done = false;

      // connection number is used to keep track
      // of the number of connections
      // accepted so far.
      // It should not be more than 100 or 200,
      // else there will be resource
      // starvation.
      while (done == false && srv->m_resman->m_nr_connections > 0)
	{
	  pdata = 0;
	  addrlen = sizeof(struct sockaddr_in);
	  err = sysapi::insock::accept(hsock, m_foreign_addr, m_hsock);
	  if (err != sysapi::error::SUCCESS)
	    {
	      done = true;
	    }
	  else
	    {
	      // Decrement the connection count
	      --srv->m_resman->m_nr_connections;

	      omode = (stmask)((int)ST_FETCHING & (int)ST_FEEDING);
	      srv->res_manager()->create(res, omode, m_foreign_addr, hsock);
	      srv->res_manager()->open(res);
	      pdata = (void*)res;

	      // Create a new session
	      proto = new net::http;
	      sess_client = new net::session(res, srv->conf(), proto);
	      srv->add_session(sess_client);
	    }
	}
    }
  // This is NOT an accepting
  // socket, reading means
  // getting data from the descriptor
  else
    {
# define BUFSZ 512
      buffer* buf = new buffer;
      sysapi::error::handle_t herr;
      buf->resize(BUFSZ);
      herr = sysapi::insock::recv(m_hsock, buf->bufptr(), (unsigned int)buf->size(), nread);
      if (herr != sysapi::error::SUCCESS)
	{
  	  if (herr == sysapi::error::CONNECTION_CLOSED)
	    m_expired = true;
	  delete buf;
	  ziafs_return_status( CANNOT_READ );
	}
      else
	{
	  buf->resize(nread);
	  m_rd_buf += *buf;
	  delete buf;
	}
    }

  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::io_on_write(void*& pdata, void*& aux)
{
  buffer* buf = (buffer*)pdata;
  unsigned int nsent;
  sysapi::error::handle_t herr;

  // Not supported for an accepting socket.
  if (m_accepting == true)
    return status::BADMODE;

  herr = sysapi::insock::send(m_hsock, buf->bufptr(), (unsigned int)buf->size(), nsent);
  if (herr != sysapi::error::SUCCESS)
    {
      if (herr == sysapi::error::CONNECTION_CLOSED)
	m_expired = true;
      ziafs_return_status( CANNOT_WRITE );
    }

  buf->remove_front(nsent);
  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::io_on_expire()
{
  // This function is called when
  // the resource manager sees
  // the resource has expired,
  // before releasing it.

  ziafs_debug_msg("resource has expired: %s\n", "");
  ziafs_return_status( SUCCESS );
}


# define DEMI_SECOND	5000000
status::error io::res_insock::io_has_expired(bool& has_expired) const
{
  unsigned long long ul_now;
  unsigned long long diff;

  // Get the current tme, should be cached
  if (m_accepting == false)
    {
      sysapi::time::current(ul_now);
      if (m_tm_lastio != 0)
	{
	  // An io resource is considered as expired when
	  // there havn't been activities for more than
	  // 500 msc.
	  diff = ul_now - m_tm_lastio;
	  if (diff > DEMI_SECOND)
	    {
	      has_expired = true;
	      ziafs_return_status( SUCCESS );
	    }
	}
    }

  has_expired = m_expired;
  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::dump(buffer&) const
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_insock::name(string& name) const
{
  name = string("<internet_socket_name>");
  ziafs_return_status( SUCCESS );
}
