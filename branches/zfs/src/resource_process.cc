//
// resource_process.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:18:15 2006 texane
// Last update Sat Feb 18 11:52:15 2006 texane
//


#include <sys/sysapi.hh>
#include <ziafs_resource.hh>


using namespace sysapi;


// resource api implementation

resource::e_error resource::process::generate(unsigned int& nbytes)
{
  return E_SUCCESS;
}


resource::e_error resource::process::flush_network(thr::pool::slot_t& thr_slot, insock::handle_t& hsock)
{
  return E_SUCCESS;
}


resource::e_error resource::process::flush_disk(sysapi::file::handle_t& hsock)
{
  return E_SUCCESS;
}


resource::e_error resource::process::flush_environ()
{
  return E_SUCCESS;
}


resource::e_error resource::process::flush_input(thr::pool::slot_t& thr_slot, buffer&)
{
  return E_SUCCESS;
}


resource::e_error resource::process::size(unsigned int& nbytes)
{
  nbytes = (unsigned int)-1;
  return E_SUCCESS;
}


// construction/destruction

resource::process::process(int ac, char** av, char** env, e_omode omode)
{
  sysapi::process::create_inoutredir_and_loadexec(proc_handle, read_handle, write_handle, ac, (const char**)av, (const char**)env);
  proc_ac = ac;
  proc_av = av;
  proc_env = env;
  generated = false;
}


resource::process::~process()
{
}
