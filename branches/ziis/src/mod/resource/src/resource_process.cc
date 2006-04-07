//
// resource_process.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 16:45:04 2006 texane
// Last update Fri Apr 07 13:25:29 2006 texane
//


#include "include/mod_resource.hh"


using namespace std;
using namespace sysapi;


// resource api implementation

resource::e_error resource::process::generate(unsigned int& nbytes, IOutput& p_out)
{
  e_error e_err;
  sysapi::process::state_t proc_st;
  sysapi::error::handle_t sys_err;
  string str_line;
  bool is_line;
  bool is_toolong;
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
      if (is_header_inprogress == true)
	{
	  e_err = resource::E_CONTINUE;
	  is_line = true;
	  while (is_line == true)
	    {
	      is_line = m_line.from_buffer(str_line, data, is_toolong);
	      data.clear();
	      if (is_line == true)
		{
		  string str_key;
		  string str_val;
		  bool is_keyval;

		  // parse the header line
		  is_keyval = http_helper::parse_cgi_header(str_line, str_key, str_val);

		  if (str_key == "transfer_encoding" ||
		      str_key == "content-encoding" ||
		      str_key == "content-length"
		      )
		    {
		      // skip those ones
		    }
		  else if (str_line == "")
		    {
		      p_out.SendHeader();
		      is_header_inprogress = false;
		      is_line = false;
		      m_line.get_bytes(data, nbytes);
		      if (data.size() == 0)
			e_err = resource::E_CONTINUE;
		      else
			e_err = resource::E_SUCCESS;
		    }
		  else if (is_keyval == true)
		    {
		      // parse the field
		      p_out.SetOutput(str_key.c_str(), str_val.c_str());
		    }
		}
	    }
	}
    }

  return e_err;
}


resource::e_error resource::process::flush_network(IOutput& out)
{
  int nr_sent;

  nr_sent = out.SendBuffer((const char*)data.bufptr(), (int)data.size());
  if (nr_sent < 0)
    return E_OP_ERROR;
  data.clear();
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
  unsigned int nsent;
  sysapi::error::handle_t sys_err;

  sys_err = sysapi::file::write(read_handle, buf.bufptr(), (unsigned int)buf.size(), nsent);
  if (buf.size() > in_size)
    in_size = 0;
  else
    in_size -= (int)buf.size();
  buf.clear();
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


bool resource::process::is_header_dynamic() const
{
  return true;
}


// construction/destruction

resource::process::process(int ac, char** av, char** env, e_omode omode)
{
  sysapi::error::handle_t sys_err;

  is_header_inprogress = true;
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
