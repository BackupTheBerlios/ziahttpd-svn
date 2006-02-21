//
// insock.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:01:50 2006 
// Last update Tue Feb 21 19:04:34 2006 
//


#include <string>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
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

void sysapi::insock::reset_handle(handle_t& sock_handle)
{
  sock_handle = -1;
}


bool sysapi::insock::handle_isset(handle_t& sock_handle)
{
  return !(sock_handle == -1);
}


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
      ::close(hsock);
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



#ifdef __NetBSD__
#include <sys/mman.h>
static int sendfile(int sock_handle, int file_handle, off_t* off, unsigned int nbytes)
{
  // semantic is as follow: the send file
  // stores the bytes count written so far
  // in nbytes, and makes off pointer points
  // at the bytes following the last we written
  // On error, -1 is returned

  sysapi::error::handle_t sys_err;
  unsigned int nsent;
  unsigned int nr;
  unsigned char* ptr;
  bool done;

  // create a mapping of the file
  ptr = (unsigned char*)mmap(0, nbytes, PROT_READ, MAP_FILE, file_handle, *off);
  if (ptr == MAP_FAILED)
    return -1;

  // send the file
  done = false;
  nsent = 0;
  while (done == false)
    {
      if (nsent >= nbytes)
	{
	  done = true;
	}
      else
	{
	  sys_err = sysapi::insock::send(sock_handle, ptr + nsent, nbytes - nsent, nr);
	  if (sys_err != sysapi::error::SUCCESS)
	    {
	      *off = 0;
	      done = true;
	      nsent = (unsigned int)-1;
	    }
	  else
	    {
	      nsent += nr;
	    }
	}
    }

  // release the mapping
  munmap(ptr, nbytes);
  return (int)nsent;
}
#else
# include <sys/sendfile.h>
#endif // __NetBSD__

sysapi::error::handle_t sysapi::insock::send_file(insock::handle_t& sock_handle, file::handle_t& file_handle, unsigned int file_size, unsigned char* buf, unsigned int nbytes)
{
  unsigned int nsent;
  off_t off;
  int nret;
  bool done;

  // First send the buffer
  nsent = 0;
  while (nsent < nbytes)
    {
      nret = ::send(sock_handle, (const char*)buf, nbytes - nsent, 0);
      if (nret <= -1)
	return error::UNKNOWN;
      nsent += (unsigned int)nret;
    }

  // Send the file
  nbytes = file_size;
  nsent = 0;
  off = 0;
  done = false;
  while (done == false)
    {
      nret = sendfile(sock_handle, file_handle, &off, nbytes);
      if (nret == -1)
	return error::WRITE_FAILED;
      nbytes -= (unsigned int)nret;
      if (nbytes == 0)
	done = true;
    }

  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::close(handle_t& hsock)
{
  shutdown(hsock, SHUT_RDWR);
  ::close(hsock);
  return error::SUCCESS;
}
