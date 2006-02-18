//
// process.cc<3> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Feb 18 10:56:19 2006 texane
// Last update Sat Feb 18 11:12:16 2006 texane
//


#include <sys/sysapi.hh>


sysapi::error::handle_t sysapi::process::create_and_loadexec(handle_t& hdl, int ac, const char** av, const char** env)
{
  return error::UNKNOWN;
}


sysapi::error::handle_t sysapi::process::create_outredir_and_loadexec(handle_t& child_hdl, sysapi::file::handle_t& read_hdl, int ac, const char** av, const char** env)
{
  return error::UNKNOWN;
}


sysapi::error::handle_t sysapi::process::create_inoutredir_and_loadexec(handle_t& child_hdl, sysapi::file::handle_t& hread, sysapi::file::handle_t& hwrite, int ac, const char** av, const char** env)
{
  return error::UNKNOWN;
}



sysapi::error::handle_t sysapi::process::wait_single(handle_t& hdl, state_t& st, waitopt_t wopt)
{
  return error::UNKNOWN;
}


sysapi::error::handle_t sysapi::process::terminate(handle_t& hdl)
{
  return error::UNKNOWN;
}


sysapi::error::handle_t sysapi::process::release(handle_t& proc_handle)
{
  return error::UNKNOWN;
}
