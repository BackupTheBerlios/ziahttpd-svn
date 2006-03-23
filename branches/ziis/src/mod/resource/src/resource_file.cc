//
// resource_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 11:02:02 2006 texane
// Last update Thu Mar 23 13:31:51 2006 texane
//


#include <string>
#include <iostream>
#include <sys/sysapi.hh>
#include <ziis.hh>
#include "include/constants.hh"
#include "include/resource.hh"
#include "include/buffer.hh"


using namespace sysapi;
using namespace std;


bool resource::file::is_content_dynamic() const
{
  return false;
}


resource::e_error resource::file::generate(unsigned int& nbytes)
{
  unsigned char buf[constants::BUFFER_SIZE];
  sysapi::error::handle_t h_err;
  resource::e_error e_err;

  e_err = E_SUCCESS;

  // put on the disk
  if (omode == O_OUTPUT)
    {
      sysapi::error::handle_t sys_err;
      unsigned int nr_written;
      bool done;

      done = false;
      while (done == false)
	{
	  if (data.size() == 0)
	    {
	      done = true;
	    }
	  else
	    {
	      sys_err = sysapi::file::write(file_handle, data.bufptr(), (unsigned int)data.size(), nr_written);
	      if (sys_err != sysapi::error::SUCCESS)
		done = true;
	      else
		data.remove_front(nr_written);
	    }
	}
      e_err = E_CONTINUE;
    }
  // send on the wire
  else if (omode == O_INPUT)
    {
      // get the file
      if (generated == true)
	{
	  e_err = E_ALREADY_GEN;
	}
      else
	{
	  // read next buffer from file
	  h_err = sysapi::file::read(file_handle, buf, sizeof(buf), nbytes);
	  if (h_err != sysapi::error::SUCCESS)
	    return E_OP_ERROR;
	  data = buffer(buf, nbytes);
	  file_size -= nbytes;
	  if (file_size == 0)
	    generated = true;
	}
    }
  else
    {
      // operation not supported yet
      e_err = E_NOT_SUPP;
    }
  return e_err;
}


resource::e_error resource::file::flush_network(IOutput& out)
{
  int nr_sent;

  if (omode == O_INPUT)
    {
      nr_sent = out.SendBuffer((const char*)data.bufptr(), (int)data.size());
      if (nr_sent < 0)
	return E_OP_ERROR;
      return E_SUCCESS;
    }

  return E_NOT_SUPP;
}


resource::e_error resource::file::flush_disk(sysapi::file::handle_t&)
{
  return E_NOT_IMPL;
}

resource::e_error resource::file::flush_environ()
{
  return E_NOT_IMPL;
}

resource::e_error resource::file::flush_input(buffer& buf)
{
  // operation not supported
  if (omode != O_OUTPUT)
    return E_NOT_SUPP;

  // append to data
  data += buf;
  buf.clear();
  return E_SUCCESS;
}

resource::e_error resource::file::size(unsigned int& nbytes)
{
  nbytes = (unsigned int)file_size;
  return E_SUCCESS;
}


// construction/destruction

resource::file::file(const string& path, e_omode omode)
{
  sysapi::error::handle_t herr;

  opened = false;
  file_path = path;
  generated = false;

  if (omode == O_INPUT)
    herr = sysapi::file::open(file_handle, path, sysapi::file::O_READ);
  else if (omode == O_OUTPUT)
    herr = sysapi::file::open(file_handle, path, sysapi::file::O_WRITE);
  else
    herr = sysapi::error::OPEN_FAILED;

  if (herr == sysapi::error::SUCCESS)
    {
      opened = true;
      sysapi::file::size(file_handle, file_size);
    }
  else
    {
      printf("error opening the file: %s\n", path.c_str());
      fflush(stdout);
    }
}

resource::file::~file()
{
  if (opened == true)
    sysapi::file::close(file_handle);
}
