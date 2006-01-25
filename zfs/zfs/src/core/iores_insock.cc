//
// iores_insock.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Jan 24 21:08:13 2006 texane
// Last update Wed Jan 25 12:15:44 2006 texane
//


#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_io.hh>
#include <core/ziafs_status.hh>


using std::string;
using namespace status;


io::res_insock::res_insock(stmask openmod, const string& local_addr, unsigned short local_port) : resource(openmod)
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
io::res_insock::res_insock(stmask openmod, const struct sockaddr_in& foreign_addr, const sysapi::insock::handle_t& hsock) : resource(openmod)
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


status::error io::res_insock::io_on_read(void*& pdata)
{
  sysapi::insock::handle_t hsock;
  resource* res;
  stmask omode;
  int addrlen;
  int nread;

  // This is an accepting socket,
  // reading means accetping an
  // incoming connection.
  if (m_accepting == true)
    {
      pdata = 0;
      addrlen = sizeof(struct sockaddr_in);
      sysapi::insock::accept(hsock, m_foreign_addr, m_hsock);
      omode = (stmask)((int)ST_FETCHING & (int)ST_FEEDING);
      res = new res_insock(omode, m_foreign_addr, hsock);
      pdata = (void*)res;
    }
  // This is NOT an accepting
  // socket, reading means
  // getting data from the descriptor
  else
    {
# define BUFSZ 256
      buffer* buf = new buffer;

      buf->resize(BUFSZ);
      nread = ::recv(m_hsock, (char*)buf->bufptr(), (int)buf->size(), 0);
      if (nread > 0)
	{
	  buf->resize(nread);
	}
      pdata = (void*)buf;
    }

  ziafs_return_status( SUCCESS );
}


status::error io::res_insock::io_on_write()
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_insock::io_on_expire()
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_insock::io_has_expired(bool& has_expired) const
{
  static unsigned int count = 0;

  has_expired = false;
  ++count;
  if (count == 100)
    has_expired = true;

  ziafs_return_status( PARTIALIMPL );
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
