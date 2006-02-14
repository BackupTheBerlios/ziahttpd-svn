//
// module.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Jan 26 00:43:21 2006 texane
// Last update Thu Jan 26 01:18:36 2006 texane
//


#define _WIN32_WINNT 0x0501

#include <string>
#include <sys/sysapi.hh>
#include <windows.h>


using std::string;


sysapi::error::handle_t sysapi::module::load(handle_t& hmod, const string& mod_name)
{
  hmod = LoadLibraryEx(reinterpret_cast<LPCTSTR>(mod_name.c_str()), 0, 0);
  if (hmod == NULL)
    return error::OPEN_FAILED;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::module::unload(handle_t& hmod)
{
  FreeLibrary(hmod);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::module::resolve(void*& sym_addr, handle_t& hmod, const std::string& sym_name)
{
  FARPROC addr;

  addr = GetProcAddress(hmod, reinterpret_cast<LPCSTR>(sym_name.c_str()));
  if (addr == NULL)
    return error::RESOLV_FAILED;
  sym_addr = reinterpret_cast<void*>(addr);
  return error::SUCCESS;
}
