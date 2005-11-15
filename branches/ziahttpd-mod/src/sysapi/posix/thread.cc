//
// thread.cc<2> for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:35:18 2005 
// Last update Sun Nov 13 15:01:01 2005 
//


#include <iostream>
#include <sysapi/posix.hh>


using std::cout;
using std::endl;


bool	posix::thread::create_and_exec(handle_t* hdl, entry_t entry, param_t param)
{
  if (::pthread_create(hdl, 0, entry, param))
    return false;

  return true;
}


bool	posix::thread::attach_to_process(handle_t, posix::process::handle_t)
{
  return false;
}


bool	posix::thread::detach_from_process(handle_t, posix::process::handle_t)
{
  return false;
}


bool	posix::thread::wait_single(handle_t hdl, state_t* state, waitopt_t wopt)
{
  state_t st;

  if (wopt == posix::thread::WAITFOREVER)
    {
      if (pthread_join(hdl, 0))
	return false;
      st = posix::thread::TERMINATED;
    }
  else
    return false;

  if (state)
    *state = st;

  return true;
}

bool	posix::thread::myhandle(handle_t* hdl)
{
  *hdl = pthread_self();
  return false;
}


bool	posix::thread::release(handle_t hdl)
{
  int ret;

  if ((ret = pthread_detach(hdl)))
    {
      // The thread is already done, don't consider as an error
      if (ret == ESRCH)
	return true;
      return false;
    }

  return true;
}


bool	posix::thread::say(const char* msg)
{
  handle_t hdl;

  posix::thread::myhandle(&hdl);
  cout << "[" << (unsigned int)hdl << "]:";
  if (msg)
    cout << msg;
  cout << endl;

  return true;
}
