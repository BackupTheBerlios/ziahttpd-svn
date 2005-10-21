//
// process.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:32:20 2005 
// Last update Sat Oct 22 03:49:40 2005 
//


#include <posix.hh>


bool	posix::process::create_and_loadexec(handle_t* child_hdl, int ac, const char** av, const char** env)
{
  // Create a child process
  *child_hdl = fork();
  if (*child_hdl == -1)
    return false;

  // We are in the child
  if (*child_hdl == 0)
    {
      execve(*av, (char* const*)av, (char* const*)env);
      exit(-1);
    }

  return true;
}


bool	posix::process::create_outredir_and_loadexec(handle_t*, posix::file::handle_t*, int, const char**, const char**)
{
  
  return false;
}


bool	posix::process::myhandle(handle_t* hdl)
{
  *hdl = getpid();
  return true;
}


bool	posix::process::signal(handle_t, sigid_t)
{
  return false;
}


bool	posix::process::release(handle_t)
{
  // always return true
  return true;
}


bool	posix::process::wait_single(handle_t, state_t* , waitopt_t)
{
  return false;
}


bool	posix::process::wait_any(handle_t*, state_t*, waitopt_t)
{
  return false;
}
