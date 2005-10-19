//
// inaddr.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Oct 13 18:46:09 2005 texane
// Last update Wed Oct 19 18:50:15 2005 
//


#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<sys/socket.h>
#include	<netdb.h>


int		getinaddr_tobuf(unsigned int *buf, const char *addr)
{
  struct hostent *he;
  int		err;

  err = inet_pton(AF_INET, addr, (void *)buf);
  if (!err)
    {
      he = gethostbyname(addr);
      if (!he)
	return (-1);
      *buf = *((unsigned int *)he->h_addr_list[0]);
    }
  if (err >= 0)
    return (0);
  return (-1);
}
