//
// resource_process.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:18:15 2006 texane
// Last update Sun Feb 19 00:25:42 2006 texane
//


#include <sys/sysapi.hh>
#include <ziafs_resource.hh>
#include <ziafs_static.hh>


using namespace sysapi;


// resource api implementation

resource::e_error resource::process::generate(unsigned int& nbytes)
{
  e_error e_err;
  sysapi::process::state_t proc_st;
  sysapi::error::handle_t sys_err;

  if (generated == true)
    return E_ALREADY_GEN;

  sys_err = sysapi::process::wait_single(proc_handle, proc_st, sysapi::process::DONTWAIT);
  if (sys_err == sysapi::error::SUCCESS)
    {
      // The process is done. There
      // may be data in the pipe, so
      // do the last read.
      generated = true;
    }

  nbytes = 0;
  e_err = E_SUCCESS;
  data.resize(ZIAFS_STATIC_BUFSZ);
  sys_err = sysapi::file::read(write_handle, data.bufptr(), (unsigned int)data.size(), nbytes);
  if (sys_err != sysapi::error::SUCCESS)
    {
      // check non blocking mode here
      e_err = E_OP_ERROR;
      generated = true;
    }
  else
    {
      data.resize(nbytes);
    }
  return e_err;
}


#include <iostream>
using namespace std;


resource::e_error resource::process::flush_network(thr::pool::slot_t& thr_slot, insock::handle_t& hsock)
{
  e_error eerr;
  error::handle_t herr;
  unsigned int nsent;
  unsigned int nbytes;
  bool done;

  done = false;
  eerr = E_SUCCESS;
  while (done == false)
    {
      nbytes = (unsigned int)data.size();
      if (nbytes == 0)
	{
	  done = true;
	}
      else
	{
	  cout << data.tostring() << endl;
	  herr = send(thr_slot, hsock, data.bufptr(), nbytes, nsent);
	  if (thr_slot.curr_io.timeouted == true || herr != sysapi::error::SUCCESS)
	    {
	      eerr = E_OP_ERROR;
	      done = true;
	    }
	  else
	    {
	      data.remove_front(nsent);
	    }
	}
    }
  return eerr;
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
  sysapi::error::handle_t sys_err;

  generated = false;
  sys_err = sysapi::process::create_inoutredir_and_loadexec(proc_handle, write_handle, read_handle, ac, (const char**)av, (const char**)env);
  if (sys_err != sysapi::error::SUCCESS)
    generated = true;
}


resource::process::~process()
{
  // ! pay attention to already closed handle...
  sysapi::process::release(proc_handle);
  sysapi::file::close(read_handle);
  sysapi::file::close(write_handle);
}
