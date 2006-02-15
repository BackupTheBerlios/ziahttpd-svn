//
// insock.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:01:50 2006 
// Last update Wed Feb 15 02:50:32 2006 
//


#include <string>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/sysapi.hh>


using std::string;


// Internals

static void set_basic_sockopts(int fd)
{
  static char optval = 1;

  // fcntl(fd, F_SETFL, O_NONBLOCK);
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
#ifdef SO_REUSEPORT
  setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
#endif // SO_REUSEPORT
}

static bool getinaddr_tobuf(unsigned int* buf, const char* addr)
{
  struct hostent* he;
  int err;

  err = inet_pton(AF_INET, addr, (void*)buf);
  if (err == 0)
    {
      he = gethostbyname(addr);
      if (he == 0)
	return false;
      *buf = *((unsigned  int*)he->h_addr_list[0]);
    }
  if (err >= 0)
    return true;
  return false;
}

static void fill_inaddr(unsigned short local_port,
			unsigned long local_addr,
			struct sockaddr_in* inaddr)
{
  inaddr->sin_family = PF_INET;
  inaddr->sin_port = local_port;
  inaddr->sin_addr.s_addr = local_addr;
}


// Exported

sysapi::error::handle_t sysapi::insock::init_subsystem()
{
  signal(SIGPIPE, SIG_IGN);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::release_subsystem()
{
  return  error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::p_to_inaddr(struct sockaddr_in& inaddr, const string& addr, unsigned short port)
{
  unsigned int bufaddr;
  if (getinaddr_tobuf(&bufaddr, addr.c_str()) == false)
    return error::UNKNOWN;
  fill_inaddr(htons(port), bufaddr, &inaddr);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::n_to_inaddr(struct sockaddr_in& inaddr, unsigned long addr, unsigned short port)
{
  fill_inaddr(htons(port), htonl(addr), &inaddr);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::create_listening(handle_t& hsock, struct sockaddr_in& inaddr, unsigned int nbklog)
{
  int ret;
  
  hsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (hsock == -1)
    return error::OPEN_FAILED;
  ret = bind(hsock, (struct sockaddr*)&inaddr, sizeof(struct sockaddr_in));
  if (ret == -1)
    {
      close(hsock);
      return error::OPEN_FAILED;
    }
  listen(hsock, nbklog);
  set_basic_sockopts(hsock);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::accept(handle_t& hsock, struct sockaddr_in& inaddr, handle_t& hbound)
{
  socklen_t addrlen;

  addrlen = sizeof(struct sockaddr_in);
  errno = 0;
  hsock = accept(hbound, (struct sockaddr*)&inaddr, &addrlen);
  if (hsock == -1)
    {
      switch (errno)
	{
	case ECONNABORTED:
	  return error::CONNECTION_ABORTED;
	  break;
	default:
	  return error::UNKNOWN;
	  break;
	}
    }
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::recv(handle_t& hsock, unsigned char* buf, unsigned int nbytes, unsigned int& nrecv)
{
  int nret;

  nret = ::recv(hsock, buf, nbytes, 0);
  if (nret <= -1)
    return error::UNKNOWN;
  nrecv = (unsigned int)nret;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::send(handle_t& hsock, unsigned char* buf, unsigned int nbytes, unsigned int& nsent)
{
  int nret;

  nret = ::send(hsock, (const char*)buf, nbytes, 0);
  if (nret <= -1)
    return error::UNKNOWN;
  nsent = (unsigned int)nret;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::close(handle_t& hsock)
{
  shutdown(hsock, SHUT_RDWR);
  // on close, the process segfault
  return error::SUCCESS;
}
