//
// process.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:32:20 2005 
// Last update Mon Oct 17 18:35:08 2005 
//


#include <posix.hh>


bool	posix::process::create_and_loadexec(handle_t*, int, const char**, const char**)
{
  return false;
}


bool	posix::process::myhandle(handle_t*)
{
  return false;
}


bool	posix::process::signal(handle_t, sigid_t)
{
  return false;
}


bool	posix::process::release(handle_t)
{
  return false;
}


bool	posix::process::wait_single(handle_t, state_t* , waitopt_t)
{
  return false;
}


bool	posix::process::wait_any(handle_t*, state_t*, waitopt_t)
{
  return false;
}
