//
// mutex.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Oct 13 16:33:13 2005 texane
// Last update Sun Nov 13 15:01:49 2005 
//


#include <sysapi/win32.hh>


// This file implements mutex interface for the win32 system api
// @see win32.hh


// Create a new mutex
bool win32::mutex::create(win32::mutex::handle_t* hdl)
{
  // The created mutex is an anonymous one, that cannot be
  // inherited by children and that is not locked by the
  // callin thread.
  *hdl = ::CreateMutex(0, false, NULL);
  if (*hdl == NULL)
    return false;
  
  return true;
}


// Destroy the mutex
bool win32::mutex::destroy(win32::mutex::handle_t hdl)
{
  // The calling thread must have the ownership
  if (win32::mutex::release(hdl) == false)
    return false;

  // Decrement kernel's count for this object
  if (::CloseHandle(hdl) == FALSE)
    return false;

  return true;
}


// Wait to acquiere mutex's ownership
// This function blocks until we are the mutex's owner
// ?What to do if the WAIT_ABANDONNED is returned?
// -> release the mutex?
// -> force the ownership to us
bool win32::mutex::acquire(win32::mutex::handle_t hdl)
{
  DWORD res;

  res = WaitForSingleObject(hdl, INFINITE);
  if (res == WAIT_FAILED ||
      res == WAIT_ABANDONED)
    {
      return false;
    }

  return true;
}


// Release our mutex ownership
bool win32::mutex::release(win32::mutex::handle_t hdl)
{
  if (::ReleaseMutex(hdl) == FALSE)
    return false;

  return true;
}
