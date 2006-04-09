//
// resource_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 11:02:02 2006 texane
// Last update Mon Apr 10 00:51:51 2006 
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


bool resource::file::is_header_dynamic() const
{
  return false;
}

bool resource::file::is_content_dynamic() const
{
  return false;
}


resource::e_error resource::file::generate(unsigned int& nbytes, IOutput& p_out)
{
  resource::e_error e_err;

  e_err = E_SUCCESS;

  // put on the disk
  if (omode == O_OUTPUT)
    {
      sysapi::error::handle_t sys_err;
      unsigned int nr_written;
      unsigned int nr_todel;
      bool done;

      done = false;
      nr_todel = 0;
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
		{
		  data.remove_front(nr_written);
		  nr_todel += nr_written;
		}
	    }
	}
      if (nr_todel > in_size)
	in_size = 0;
      else
	in_size -= nr_todel;
      e_err = E_CONTINUE;
      if (in_size == 0)
	e_err = E_ALREADY_GEN;
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
	  // read the next one
	  p_read = h_mmap.p_mmapping;
	  nr_read = (unsigned int)h_mmap.sz_mmapping;
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
  int nr_tosend;
  int nr_sent;
  int nr_filesz;
  bool is_done;
  unsigned char* p_buf;

  p_buf = p_read;
  nr_filesz = nr_read;
  is_done = true;
  if (nr_filesz || generated == false)
    is_done = false;

  if (omode == O_INPUT)
    {
      // this is the last chunk request?
      if (generated == true && file_size == 0)
	{
	  out.SendBuffer((char*)"", 0);
	  is_done = true;
	}

      // send the whole resource
      while (is_done == false)
	{
	  nr_tosend = constants::FBLOCK_SIZE;
	  if (nr_filesz < nr_tosend)
	    nr_tosend = nr_filesz;
	  nr_sent = out.SendBuffer((const char*)p_buf, nr_tosend);
	  if (nr_sent >= 0)
	    {
	      // affect, compression module change
	      // returned the size, and we cannot
	      // guess the eof
	      nr_sent = nr_tosend;
	      if (nr_sent > nr_filesz)
		nr_sent = nr_filesz;
	      nr_filesz -= nr_sent;
	      p_buf += nr_sent;
	      if (nr_filesz == 0)
		{
		  is_done = true;
		  file_size = 0;
		  generated = true;
		}
	    }
	  else if (nr_sent < 0)
	    {
	      return E_OP_ERROR;
	    }
	}
    }
  else
    {
      return E_NOT_SUPP;
    }

  return E_SUCCESS;
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
    {
      herr = sysapi::mmapping::alloc(h_mmap, path);
      nr_read = 0;
      p_read = h_mmap.p_mmapping;
      herr = sysapi::file::open(file_handle, path, sysapi::file::O_READ);
    }
  else if (omode == O_OUTPUT)
    herr = sysapi::file::open(file_handle, path, sysapi::file::O_WRITE);
  else
    herr = sysapi::error::OPEN_FAILED;

  if (herr == sysapi::error::SUCCESS)
    {
      opened = true;
      sysapi::file::size(file_handle, file_size);
    }
}

resource::file::~file()
{
  if (opened == true)
    {
      sysapi::mmapping::release(h_mmap);
      sysapi::file::close(file_handle);
    }
}
