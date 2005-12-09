//
// shared_object.cc<2> for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 00:12:29 2005 
// Last update Mon Nov 14 01:21:09 2005 texane
//


// @see msdn
// To make GetModuleHandleEx available...
#define _WIN32_WINNT 0x0501

#include <string>
#include <sysapi/sysapi.hh>


using std::string;


bool	win32::shared_object::open(handle_t& hobj, const string& name, error_t&)
{
  hobj = LoadLibraryEx(reinterpret_cast<LPCTSTR>(name.c_str()), 0, 0);
  if (hobj == NULL)
    return false;

  return true;
}


bool	win32::shared_object::close(handle_t& hobj)
{
  FreeLibrary(hobj);
  return true;
}


bool	win32::shared_object::resolve(void*& symaddr, handle_t& hobj, const string& name, error_t&)
{
  FARPROC addr;

  if ((addr = GetProcAddress(hobj, reinterpret_cast<LPCSTR>(name.c_str()))) == NULL)
    return false;

  symaddr = reinterpret_cast<void*>(addr);
  return true;
}
