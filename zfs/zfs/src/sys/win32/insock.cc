//
// insock.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 10:35:30 2006 texane
// Last update Wed Jan 25 12:01:05 2006 texane
//


#include <string>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#include <sys/sysapi.hh>


using std::string;


// Internal routines

static inline int inet_pton(const char* pres_addr, unsigned long* net_addr)
{
  // Resolve the presentable address
  // pres_addr to the network one net_addr.
  // ! address is netbyte ordered.

  int ret;
  addrinfo* addri = NULL;
  addrinfo* tmp;
  addrinfo hints;

  memset(&hints, 0, sizeof(addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  ret = getaddrinfo(pres_addr, NULL, &hints, &addri);
  if (ret)
    return -1;

  *net_addr = ((struct sockaddr_in*)((*addri).ai_addr))->sin_addr.s_addr;
  while (addri)
    {
      tmp = addri;
      addri = addri->ai_next;
      freeaddrinfo(tmp);
    }

  return 0;
}

static inline bool inaddr_fill(struct sockaddr_in& inaddr, unsigned short port, unsigned long addr)
{
  SecureZeroMemory((LPVOID)&inaddr, sizeof(sockaddr));
  inaddr.sin_family = AF_INET;
  inaddr.sin_port = port;
  inaddr.sin_addr.s_addr = addr;
  return true;
}

static inline bool inaddr_tobuf(unsigned long* buf, const char* addr)
{
  // ! reentrancy: ise gethostbyname_r()

  struct hostent* he;
  int err;

  err = inet_pton(addr, buf);
  if (err == 0)
    {
      he = gethostbyname(addr);
      if (he == 0)
	err = -1;
      else
	*buf = *((unsigned int*)he->h_addr_list[0]);
    }
  return err >= 0 ? true : false;
}


static inline bool set_nonblocking_mode(SOCKET& hsock)
{
  char optval = 1;
  setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  return true;
}



// Api implementation

static bool g_winsock_initialized = false;

sysapi::error::handle_t sysapi::insock::init_subsystem()
{
  WSADATA wsa_data;

  if (g_winsock_initialized == true)
    return error::SUCCESS;

  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NOERROR)
    return error::OPEN_FAILED;
  g_winsock_initialized = true;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::release_subsystem()
{
  if (g_winsock_initialized == false)
    return error::CLOSE_FAILED;

  if (WSACleanup())
    return error::CLOSE_FAILED;
  g_winsock_initialized = false;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::p_to_inaddr(struct sockaddr_in& inaddr, const std::string& addr, unsigned short port)
{
  unsigned long buf;

  if (inet_pton(addr.c_str(), &buf) == -1)
    return error::INVALID_NAME;
  inaddr_fill(inaddr, htons(port), buf);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::n_to_inaddr(struct sockaddr_in& inaddr, unsigned long addr, unsigned short port)
{
  inaddr_fill(inaddr, htons(port), htonl(addr));
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::create_listening(handle_t& hsock, struct sockaddr_in& inaddr, unsigned int nbklog)
{
  int ret;

  hsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (hsock == SOCKET_ERROR)
    return error::OPEN_FAILED;

  // Set socket attributes
  // . non blocking
  set_nonblocking_mode(hsock);
  ret = ::bind(hsock, reinterpret_cast<struct sockaddr*>(&inaddr), sizeof(struct sockaddr_in));
  if (ret == -1)
    return error::OPEN_FAILED;
  // . listening
  if (nbklog == 0)
    nbklog = 10;
  ret = ::listen(hsock, nbklog);
  if (ret == SOCKET_ERROR)
    return error::OPEN_FAILED;

  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::accept(handle_t& hsock, struct sockaddr_in& inaddr, handle_t& hbound)
{
  int addrlen;

  addrlen = sizeof(struct sockaddr_in);
  hsock = ::accept(hbound, reinterpret_cast<struct sockaddr*>(&inaddr), &addrlen);
  if (hsock == SOCKET_ERROR)
    return error::OPEN_FAILED;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::recv(handle_t& hsock, unsigned char* buf, unsigned int nbytes, unsigned int& nrecv)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::send(handle_t&)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::close(handle_t& hsock)
{
  shutdown(hsock, 0);
  closesocket(hsock);
  return error::SUCCESS;
}
