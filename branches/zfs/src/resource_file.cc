//
// resource_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:15:23 2006 texane
// Last update Thu Feb 23 20:55:32 2006 texane
//


// File resource implementation


#include <string>
#include <sys/sysapi.hh>
#include <ziafs_static.hh>
#include <ziafs_resource.hh>


using namespace sysapi;
using std::string;


resource::e_error resource::file::generate(unsigned int& nbytes)
{
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
	  generated = true;
	  nbytes = (unsigned int)file_size;
	  e_err = E_SUCCESS;
	}
    }
  else
    {
      // operation not supported yet
      e_err = E_NOT_SUPP;
    }
  return e_err;
}


#include <iostream>
using namespace std;

resource::e_error resource::file::flush_network(thr::pool::slot_t& thr_slot, insock::handle_t& hsock)
{
  if (omode == O_INPUT)
    {
      // Send the file
      sysapi::error::handle_t sys_err;

      // initiate the io operation
      io_info_reset(thr_slot.curr_io);
      thr_slot.curr_io.sz = (unsigned int)data.size() + (unsigned int)file_size;
      thr_slot.curr_io.id = thr::IO_SENDFILE;
      thr_slot.curr_io.desc.hsock = hsock;
      thr_slot.curr_io.tm_start = thr_slot.pool->tm_now();
      thr_slot.curr_io.in_progress = true;

      // send the whole file
      sys_err = insock::send_file(hsock, file_handle, (unsigned int)file_size, data.bufptr(), (unsigned int)data.size());
      thr_slot.curr_io.in_progress = true;

      // post process
      if (thr_slot.curr_io.timeouted == true || sys_err != sysapi::error::SUCCESS)
	return E_OP_ERROR;
      return E_SUCCESS;
    }
  else if (omode == O_OUTPUT)
    {
      // send only the header
    }
  return E_NOT_SUPP;
}


resource::e_error resource::file::flush_disk(sysapi::file::handle_t& hsock)
{
  return E_NOT_IMPL;
}

resource::e_error resource::file::flush_environ()
{
  return E_NOT_IMPL;
}

resource::e_error resource::file::flush_input(thr::pool::slot_t& thr_slot, buffer& buf)
{
  cout << buf.tostring() << endl;

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
