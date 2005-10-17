//
// thread.cc<2> for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:35:18 2005 
// Last update Mon Oct 17 18:38:13 2005 
//


#include <posix.hh>


bool	posix::thread::create_and_exec(handle_t*, state_t, entry_t, param_t)
{
  return false;
}


bool	posix::thread::attach_to_process(handle_t, posix::process::handle_t)
{
  return false;
}


bool	posix::thread::detach_from_process(handle_t, posix::process::handle_t)
{
  return false;
}


bool	posix::thread::wait_single(handle_t, state_t*, waitopt_t)
{
  return false;
}


bool	posix::thread::signal(handle_t, sigid_t)
{
  return false;
}


bool	posix::thread::myhandle(handle_t*)
{
  return false;
}


bool	posix::thread::release(handle_t)
{
  return false;
}


bool	posix::thread::say(const char* msg)
{
  return false;
}
