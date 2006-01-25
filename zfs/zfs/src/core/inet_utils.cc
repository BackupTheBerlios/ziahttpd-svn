//
// insock_utils.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Jan 24 21:16:01 2006 texane
// Last update Tue Jan 24 22:28:11 2006 texane
//


#include <string>
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <core/inet_utils.hh>


using std::string;


// Internal helper routines
static inline int inet_pton(const char* pres_addr, unsigned long* net_addr)
{
  // Resolve the presentable address
  // pres_addr to the network one net_addr.

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


// Exported routines

bool inet_utils::set_nonblocking_socket(int& hsock)
{
  char optval = 1;
  setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  return true;
}


bool inet_utils::create_listening_socket(int& hsock, struct sockaddr_in& inaddr, unsigned short local_port, unsigned long local_addr, int nbklog)
{
  int ret;

  hsock = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (hsock == -1)
    return false;
  
  // Set socket attributes
  // . non blocking
  set_nonblocking_socket(hsock);
  inaddr_fill(inaddr, htons(local_port), htonl(local_addr));
  ret = bind(hsock, reinterpret_cast<struct sockaddr*>(&inaddr), sizeof(struct sockaddr_in));
  if (ret == -1)
    return false;
  // . listening
  if (nbklog == 0)
    nbklog = 10;
  ret = ::listen(hsock, nbklog);
  if (ret == SOCKET_ERROR)
    return false;

  return true;
}


bool inet_utils::create_listening_socket(int& hsock, struct sockaddr_in& inaddr, unsigned short local_port, string& local_addr, int nbklog)
{
  unsigned long addr;

  // Fill the address buffer
  if (inaddr_tobuf(&addr, local_addr.c_str()) == false)
    return false;
  return create_listening_socket(hsock, inaddr, local_port, htonl(addr), nbklog);
}


bool inet_utils::init_subsystem()
{
  WSADATA wsa_data;

  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != NOERROR)
    return false;
  return true;
}


bool inet_utils::release_subsystem()
{
  if (WSACleanup())
    return false;
  return true;
}


bool inet_utils::accept(int& hsock, struct sockaddr_in& inaddr, int& hbound)
{
  int addrlen;

  addrlen = sizeof(struct sockaddr_in);
  hsock = ::accept(hbound, reinterpret_cast<struct sockaddr*>(&inaddr), &addrlen);
  if (hsock == SOCKET_ERROR)
    return false;
  return true;
}
