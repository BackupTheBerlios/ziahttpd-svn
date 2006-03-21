//
// module.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:01:56 2006 
// Last update Sat Feb 11 18:42:15 2006 
//


#include <string>
#include <dlfcn.h>
#include <sys/sysapi.hh>


using std::string;


sysapi::error::handle_t sysapi::module::load(handle_t& hmod, const string& mod_name)
{
#if defined (linux)
# define DL_LAZY RTLD_LAZY
#endif // linux

  hmod = dlopen((const char*)mod_name.c_str(), DL_LAZY);
  if (hmod == NULL)
    return error::UNKNOWN;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::module::unload(handle_t& hmod)
{
  dlclose(hmod);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::module::resolve(void*& sym_addr, handle_t& hmod, const string& sym_name)
{
  sym_addr = dlsym(hmod, sym_name.c_str());
  if (sym_addr == NULL)
    return error::UNKNOWN;
  return error::SUCCESS;
}
