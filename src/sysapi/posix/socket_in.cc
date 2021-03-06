//
// socket_in.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:40:35 2005 
// Last update Wed Oct 26 18:09:33 2005 
//


#include <posix.hh>


// fixme : use gethostbyname_r()


// Helper routines
bool		getinaddr_tobuf(unsigned int *buf, const char *addr)
{
  struct hostent *he;
  int		err;

  err = inet_pton(AF_INET, addr, (void *)buf);
  if (!err)
    {
      he = gethostbyname(addr);
      if (!he)
	return false;
      *buf = *((unsigned int *)he->h_addr_list[0]);
    }
  if (err >= 0)
    return true;
  return false;
}

static void inline fill_inaddr(unsigned short local_port,
			       unsigned long local_addr,
			       struct sockaddr_in* inaddr)
{
  inaddr->sin_family = AF_INET;
  inaddr->sin_port = local_port;
  inaddr->sin_addr.s_addr = local_addr;
}

static void inline set_nonblocking_mode(posix::socket_in::handle_t hdl)
{
  char optval = 1;
  setsockopt(hdl, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
}



// Here, increase the maximum fd by process to ~ 80
bool posix::socket_in::init_subsystem(error_t*)
{
  signal(SIGPIPE, SIG_IGN);
  return true;
}


// Nothing to destroy
bool posix::socket_in::release_subsystem(error_t*)
{
  return true;
}


bool posix::socket_in::create_listening(handle_t* hdl,
					unsigned short local_port,
					unsigned long local_addr,
					int nr_listen,
					error_t*)
{
  struct sockaddr_in sa;
  posix::socket_in::handle_t sock;

  if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    return false;

  set_nonblocking_mode(sock);
  fill_inaddr(htons(local_port), htonl(local_addr), &sa);
  if (bind(sock, reinterpret_cast<struct sockaddr*>(&sa), sizeof(struct sockaddr_in)) == -1)
    {
      posix::socket_in::terminate_connection(sock);
      return false;
    }

  if (!nr_listen)
    nr_listen = 10;
  ::listen(sock, nr_listen);

  *hdl = sock;
  return true;
}


bool posix::socket_in::create_listening(handle_t* hdl,
					unsigned short localport,
					const char* localaddr,
					int nr_listen,
					error_t* err)
{
  unsigned int addr;

  if (getinaddr_tobuf(&addr, localaddr) == false)
    return false;

  return posix::socket_in::create_listening(hdl, localport, htonl(addr), nr_listen, err);
}


bool posix::socket_in::accept(handle_t* hdl_con,
			      handle_t hdl_accept,
			      struct sockaddr* saddr,
			      error_t*)
{
  socklen_t addrlen;
  posix::socket_in::handle_t res;

  addrlen = sizeof(struct sockaddr_in);
  res = ::accept(hdl_accept, saddr, saddr ? &addrlen : 0);
  if (res == -1)
    return false;

  *hdl_con = res;

  return true;
}


bool posix::socket_in::terminate_connection(handle_t hdl)
{
  if (shutdown(hdl, 0) == -1)
    return false;
  close(hdl);
  return true;
}


bool posix::socket_in::recv(handle_t hdl,
			    unsigned char* buf,
			    size_t sz,
			    size_t* nr_read,
			    error_t* err)
{
  int res;
  bool ret = true;

  res = ::recv(hdl, reinterpret_cast<char*>(buf), sz, 0);

  if (res <= 0)
    ret = false;

  if (nr_read)
    *nr_read = res;

  // set the error
  if (err)
    {
      // end of connection
      if (res == 0)
	{
	  *err = posix::socket_in::CONN_DISCONNECTED;
	}
      else
	{
	  *err = posix::socket_in::ERR_UNKNOWN;
	}
    }

  return ret;
}


bool posix::socket_in::send(handle_t hdl,
			    const unsigned char* buf,
			    size_t sz,
			    size_t* nr_written,
			    error_t*)
{
  int res;

  res = ::send(hdl, reinterpret_cast<const char*>(buf), sz, 0);
  if (res == -1)
    return false;

  if (nr_written)
    *nr_written = res;
  return true;
}
