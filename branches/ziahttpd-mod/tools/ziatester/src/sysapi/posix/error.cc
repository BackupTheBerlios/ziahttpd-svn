//
// error.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:46:59 2005 
// Last update Sun Nov 13 15:00:11 2005 
//


#include <iostream>
#include <sysapi/posix.hh>


posix::error::handle_t posix::error::get()
{
  return 0;
}


void	posix::error::set(handle_t)
{
}


void	posix::error::stringify(const char* msg)
{
  if (msg)
    perror(msg);
  std::cout << std::endl;
}
