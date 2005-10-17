//
// thread.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Oct 11 11:26:36 2005 texane
// Last update Mon Oct 17 13:54:41 2005 texane
//


#include <win32.hh>
#include <iostream>

bool	win32::thread::create_and_exec(handle_t* hdl, state_t state, entry_t entry, param_t param)
{
  unsigned initflag;
  unsigned thid;
  handle_t res;

  if (state == SUSPENDED)
    initflag = CREATE_SUSPENDED;
  else
    initflag = 0;

  if (_beginthreadex(0, 0, entry, param, initflag, &thid) == 0)
    {
      return false;
    }

  if ((res = OpenThread(THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION| SYNCHRONIZE, false, static_cast<DWORD>(thid))) == NULL)
    {
      _endthreadex(-1);
      return false;
    }

  *hdl = res;
  return true;
}


// Not yet implemented
bool	win32::thread::attach_to_process(handle_t, win32::process::handle_t)
{
  return false;
}


// Not yet implemented
bool	win32::thread::detach_from_process(handle_t, win32::process::handle_t)
{
  return false;
}


bool	win32::thread::signal(handle_t hdl, sigid_t sigid)
{
  bool res;

  res = true;
  switch (sigid)
    {
    case TERMINATE:
      _endthreadex(-1);
      break;

    case RESUME:
      if (ResumeThread(hdl) == (DWORD)-1)
	res = false;
      break;

    case SUSPEND:
      if (SuspendThread(hdl) == (DWORD)-1)
	res = false;
      break;

    default:
      res = false;
      break;
    }

  return res;
}


bool	win32::thread::myhandle(handle_t* hdl)
{
  *hdl = GetCurrentThread();
  return true;
}


bool	win32::thread::release(handle_t hdl)
{
  if (CloseHandle(hdl) == FALSE)
    return false;
  return true;
}


bool	win32::thread::wait_single(handle_t hdl, state_t* st, waitopt_t wopt)
{
  DWORD code;
  state_t state;

  if (WaitForSingleObject(hdl, wopt == WAITFOREVER ? INFINITE : 0) == WAIT_FAILED)
    return false;

  if (GetExitCodeThread(hdl, &code) == FALSE)
    return false;

  state = TERMINATED;

  if (code == STILL_ACTIVE && wopt == WAITFOREVER)
    return false;
  else if (code == STILL_ACTIVE && wopt == DONTWAIT)
    state = RUNNING;

  if (st)
    *st = state;

  return true;
}


bool	win32::thread::say(const char* msg)
{
  DWORD	id;

  id = GetCurrentThreadId();
  if (id == NULL)
    {
      return false;
    }
  std::cout << "[" << id << "]";
  if (msg)
    std::cout << ":"<< msg;

  std::cout << std::endl;
    
  return true;
}
