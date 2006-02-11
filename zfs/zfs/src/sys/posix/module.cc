//
// module.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:01:56 2006 
// Last update Sat Feb 11 17:22:36 2006 
//


#include <string>
#include <sys/sysapi.hh>


using std::string;


sysapi::error::handle_t sysapi::module::load(handle_t& hmod, const string& mod_name)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::module::unload(handle_t& hmod)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::module::resolve(void*& sym_addr, handle_t& hmod, const string& sym_name)
{
  return error::SUCCESS;
}
