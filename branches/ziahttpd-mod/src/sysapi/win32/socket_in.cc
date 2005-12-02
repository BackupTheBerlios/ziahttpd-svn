//
// socket_in.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 17:46:27 2005 texane
// Last update Fri Dec 02 11:18:56 2005 texane
//


#include <winsock2.h>
#include <Ws2tcpip.h>
#include <sysapi/win32.hh>


static void mkerr_from_code(win32::socket_in::error_t* err, SOCKET code)
{
  if (!err)
    return ;
}


bool win32::socket_in::init_subsystem(win32::socket_in::error_t* err)
{
  WSADATA wsa_data;

  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NOERROR)
    {
      return false;
    }
  return true;
}


bool win32::socket_in::release_subsystem(win32::socket_in::error_t* err)
{
  if (WSACleanup())
    return false;
  return true;
}


static bool resolve_readable_inaddr(const char* addr, unsigned long* res)
{
  int ret;
  addrinfo* addri = NULL;
  addrinfo* tmp;
  addrinfo hints;

  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  ret = getaddrinfo(addr, NULL, &hints, &addri);
  if (ret)
    return false;

  *res = ((struct sockaddr_in*)((*addri).ai_addr))->sin_addr.s_addr;

  while (addri)
    {
      tmp = addri;
      addri = addri->ai_next;
      freeaddrinfo(tmp);
    }

  return true;
}

static void inline fill_inaddr(unsigned short local_port,
			       unsigned long local_addr,
			       struct sockaddr_in* inaddr)
{
  inaddr->sin_family = AF_INET;
  inaddr->sin_port = local_port;
  inaddr->sin_addr.s_addr = local_addr;
}

static void inline set_nonblocking_mode(win32::socket_in::handle_t hdl)
{
  char optval = (char)TRUE;
  setsockopt(hdl, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}

bool win32::socket_in::create_listening(win32::socket_in::handle_t* hdl,
					unsigned short local_port,
					unsigned long local_addr,
					int nr_listen,
					win32::socket_in::error_t* err)
{
  struct sockaddr_in sa;
  win32::socket_in::handle_t sock;

  if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == SOCKET_ERROR)
    return false;

  set_nonblocking_mode(sock);
  fill_inaddr(htons(local_port), htonl(local_addr), &sa);
  if (bind(sock, reinterpret_cast<struct sockaddr*>(&sa), sizeof(struct sockaddr_in)) == SOCKET_ERROR)
    {
      win32::socket_in::terminate_connection(sock);
      return false;
    }

  if (nr_listen)
    listen(sock, nr_listen);
  else
    listen(sock, 10);

  *hdl = sock;
  return true;
}


bool win32::socket_in::create_listening(win32::socket_in::handle_t* hdl,
					unsigned short localport,
					const char* localaddr,
					int nr_listen,
					win32::socket_in::error_t* err)
{
  unsigned long addr;

  if (resolve_readable_inaddr(localaddr, &addr) == false)
    {
      return false;
    }
  return win32::socket_in::create_listening(hdl, localport, htonl(addr), nr_listen, err);
}


bool win32::socket_in::accept(win32::socket_in::handle_t* hdl_con,
			      win32::socket_in::handle_t hdl_accept,
			      struct sockaddr* saddr,
			      win32::socket_in::error_t* err)
{
  int addrlen;
  win32::socket_in::handle_t res;

  addrlen = sizeof(struct sockaddr_in);
  res = ::accept(hdl_accept, saddr, saddr ? &addrlen : NULL);
  if (res == INVALID_SOCKET)
    {
      return false;
    }

  *hdl_con = res;

  return true;
}

bool win32::socket_in::terminate_connection(win32::socket_in::handle_t hdl)
{
  if (shutdown(hdl, 0) == SOCKET_ERROR)
    return false;
  if (closesocket(hdl) == SOCKET_ERROR)
    return false;
  return true;
}


bool win32::socket_in::send(win32::socket_in::handle_t hdl,
			    const unsigned char* buf,
			    win32::socket_in::size_t sz,
			    win32::socket_in::size_t* nr_written,
			    win32::socket_in::error_t* err)
{
  int res;

  res = ::send(hdl, reinterpret_cast<const char*>(buf), sz, 0);
  if (res == SOCKET_ERROR)
    return false;

  if (nr_written)
    *nr_written = res;
  return true;
}

bool win32::socket_in::recv(win32::socket_in::handle_t hdl,
			    unsigned char* buf,
			    win32::socket_in::size_t sz,
			    win32::socket_in::size_t* nr_read,
			    win32::socket_in::error_t* err)
{
  int res;
  int wsa_error;
  error_t reason;

  reason = SUCCESS;

  res = ::recv(hdl, reinterpret_cast<char*>(buf), sz, 0);
  if (res == SOCKET_ERROR)
    {
      reason = ERR_UNKNOWN;
      wsa_error = WSAGetLastError();
      switch (wsa_error)
	{
	case WSAENETRESET:
	case WSAESHUTDOWN:
	case WSAECONNABORTED:
	case WSAECONNRESET:
	  reason = CONN_DISCONNECTED;
	  break;
	}
      if (err)
	*err = reason;
      return false;
    }

  // The connection has been closed
  if (res == 0)
    reason = CONN_DISCONNECTED;

  // Store the read byte count
  if (nr_read)
    *nr_read = res;

  // The caller wants to know about the error
  if (err)
    *err = reason;
  
  return true;
}
