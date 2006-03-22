//
// resource_process.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 16:45:04 2006 texane
// Last update Wed Mar 22 16:56:55 2006 texane
//


#include "include/mod_resource.hh"


using namespace std;
using namespace sysapi;


// resource api implementation

resource::e_error resource::process::generate(unsigned int& nbytes)
{
  e_error e_err;
  sysapi::process::state_t proc_st;
  sysapi::error::handle_t sys_err;
  unsigned char buf[constants::BUFFER_SIZE];

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


resource::e_error resource::process::flush_network(IOutput& out)
{
  int nr_sent;

  nr_sent = out.SendBuffer((const char*)data.bufptr(), (int)data.size());
  if (nr_sent < 0)
    return E_OP_ERROR;
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


resource::e_error resource::process::flush_input(buffer& buf)
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
  nbytes = 0;
  return E_SUCCESS;
}


bool resource::process::is_content_dynamic() const
{
  return true;
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
