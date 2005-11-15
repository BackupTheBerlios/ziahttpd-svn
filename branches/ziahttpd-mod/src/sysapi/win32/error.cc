//
// error.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Mon Oct 10 17:02:42 2005 texane
// Last update Sun Nov 13 15:01:13 2005 
//


#include <iostream>
#include <sysapi/win32.hh>


// !Fix: inline those functions


win32::error::handle_t win32::error::get()
{
  return GetLastError();
}


void	win32::error::set(win32::error::handle_t err)
{
  // Not yet implmented
}


void win32::error::stringify(const char* prefix)
{
#define NR_MAX_BUF	256
  char buf[NR_MAX_BUF];
  DWORD ret;

  ret = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, get(), 0, buf, sizeof(buf) - 1, 0);
  if (ret)
    {
      if (prefix)
	std::cout << prefix;
      std::cout << buf << std::endl;
    }
}
