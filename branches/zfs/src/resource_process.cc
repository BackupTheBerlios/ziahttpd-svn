//
// resource_process.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:18:15 2006 texane
// Last update Thu Feb 23 02:30:48 2006 texane
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
  unsigned char buf[ZIAFS_STATIC_BUFSZ];

  if (generated == true)
    return E_ALREADY_GEN;

  sys_err = sysapi::process::wait_single(proc_handle, proc_st, sysapi::process::DONTWAIT);
  if (sys_err == sysapi::error::SUCCESS)
    {
      // The process is done. There
      // may be data in the pipe, so
      // do the last read.
      // generated = true;
    }

  nbytes = 0;
  e_err = E_SUCCESS;
  sys_err = sysapi::file::read_nonblock(write_handle, buf, sizeof(buf), nbytes);
  if (sys_err == sysapi::error::OPERATION_WOULDBLOCK)
    {
      // blocking operation
      nbytes = 0;
      data.clear();
      e_err = resource::E_WOULDBLOCK;
    }
  else if (sys_err != sysapi::error::SUCCESS)
    {
      // error
      e_err = E_OP_ERROR;
      generated = true;
      data.clear();
    }
  else
    {
      // success
      data = buffer(buf, nbytes);
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


resource::e_error resource::process::flush_input(thr::pool::slot_t& thr_slot, buffer& buf)
{
  bool done;
  unsigned int nsent;
  sysapi::error::handle_t sys_err;

  done = false;
  while (done == false)
    {
      if (buf.size() == 0)
	{
	  done = true;
	}
      else
	{
	  sys_err = sysapi::file::write(read_handle, buf.bufptr(), (unsigned int)buf.size(), nsent);
	  if (sys_err != sysapi::error::SUCCESS)
	    {
	      done = true;
	    }
	  else
	    {
	      in_size -= nsent;
	      buf.remove_front(nsent);
	    }
	}
    }
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
