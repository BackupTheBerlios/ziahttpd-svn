//
// iores_insock.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Jan 24 21:08:13 2006 texane
// Last update Sat Jan 28 15:51:37 2006 texane
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

  if (m_accepting)
    {
      if (sysapi::insock::p_to_inaddr(m_local_addr, m_my_addr, m_my_port) != sysapi::error::SUCCESS)
	ziafs_return_status( CANNOT_OPEN );
      if (sysapi::insock::create_listening(m_hsock, m_local_addr, 10) != sysapi::error::SUCCESS)
	ziafs_return_status( CANNOT_OPEN );
    }

  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::io_on_close()
{
  sysapi::insock::close(m_hsock);
  ziafs_return_status( PARTIALIMPL );
}


status::error io::res_insock::io_on_read(void*& pdata, void*& aux)
{
  net::server* srv;
  net::session* sess_client;
  net::protocol* proto;
  sysapi::insock::handle_t hsock;
  resource* res;
  stmask omode;
  unsigned int nread;
  int addrlen;

  ziafs_debug_msg("read request: %d\n", m_rd_buf.size());

  srv = (net::server*&)aux;

  // This is an accepting socket,
  // reading means accetping an
  // incoming connection.
  if (m_accepting == true)
    {
      ziafs_debug_msg("handling incoming connection %s\n", "");

      // Create the new resource
      pdata = 0;
      addrlen = sizeof(struct sockaddr_in);
      sysapi::insock::accept(hsock, m_foreign_addr, m_hsock);
      omode = (stmask)((int)ST_FETCHING & (int)ST_FEEDING);
      res = new res_insock(omode, m_foreign_addr, hsock);
      pdata = (void*)res;

      // Create a new session
      proto = new net::http;
      ziafs_debug_msg("[!] if you see this, tell %s", "texane@gmail.com");
      sess_client = new net::session(res, srv->conf(), proto);
      srv->add_session(sess_client);
    }
  // This is NOT an accepting
  // socket, reading means
  // getting data from the descriptor
  else
    {
# define BUFSZ 1
      buffer* buf = new buffer;
      sysapi::error::handle_t herr;
      buf->resize(BUFSZ);
      herr = sysapi::insock::recv(m_hsock, buf->bufptr(), (unsigned int)buf->size(), nread);
      if (herr != sysapi::error::SUCCESS)
	{
	  if (herr == sysapi::error::CONNECTION_CLOSED)
	    m_expired = true;
	  ziafs_return_status( CANNOT_READ );
	}
      else
	{
	  buf->resize(nread);
	}
      pdata = (void*)buf;
    }

  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::io_on_write(void*& pdata, void*& aux)
{
  buffer* buf = (buffer*)pdata;
  unsigned int nsent;
  sysapi::error::handle_t herr;

  ziafs_debug_msg("write request: %d\n", buf->size());

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

  ziafs_debug_msg("has written %d bytes\n", nsent);
  buf->remove_front(nsent);
  ziafs_return_status( SUCCESS );
}


#include <iostream>
using namespace std;
status::error io::res_insock::io_on_expire()
{
  // This function is called when
  // the resource manager sees
  // the resource has expired,
  // before releasing it.

  cout << "the resource expired!" << endl;
  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::io_has_expired(bool& has_expired) const
{
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
