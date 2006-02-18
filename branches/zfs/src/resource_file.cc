//
// resource_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:15:23 2006 texane
// Last update Sat Feb 18 02:58:45 2006 texane
//


// File resource implementation


#include <string>
#include <sys/sysapi.hh>
#include <ziafs_static.hh>
#include <ziafs_resource.hh>


using namespace sysapi;
using std::string;


// resource::e_error resource::file::generate(unsigned int& nbytes)
// {
//   sysapi::error::handle_t herr;
//   unsigned int nread;
//   unsigned char buf[ZIAFS_STATIC_BUFSZ];

//   // basic checks
//   nbytes = 0;
//   if (opened == false)
//     return E_NOT_OPENED;
//   if (generated == true)
//     return E_ALREADY_GEN;

//   generated = true;
      
//   // read the whole file into memory
//   while (data.size() < file_size)
//     {
//       herr = sysapi::file::read(file_handle, (unsigned char*)buf, sizeof(buf), nread);
//       if (herr != sysapi::error::SUCCESS)
// 	{
// 	  data.reset();
// 	  return E_OP_ERROR;
// 	}
//       data += buffer((unsigned char*)buf, nread);
//       nbytes += nread;
//     }
//   nbytes = (unsigned int)data.size();
//   return E_SUCCESS;
// }

resource::e_error resource::file::generate(unsigned int& nbytes)
{
  if (generated == true)
    return E_ALREADY_GEN;
  generated = true;
  nbytes = (unsigned int)file_size;
  return E_SUCCESS;
}

resource::e_error resource::file::flush_network(thr::pool::slot_t& thr_slot, insock::handle_t& hsock)
{
  // Send the file
  sysapi::error::handle_t sys_err;

  // initiate the io operation
  io_info_reset(thr_slot.curr_io);
  thr_slot.curr_io.sz = (unsigned int)data.size();
  thr_slot.curr_io.id = thr::IO_SENDFILE;
  thr_slot.curr_io.desc.hsock = hsock;
  thr_slot.curr_io.tm_start = thr_slot.pool->tm_now();
  thr_slot.curr_io.in_progress = true;

  // send the whole file
  sys_err = insock::send_file(hsock, file_handle, data.bufptr(), (unsigned int)data.size());
  thr_slot.curr_io.in_progress = true;

  // post process
  if (thr_slot.curr_io.timeouted == true || sys_err != sysapi::error::SUCCESS)
    return E_OP_ERROR;
  return E_SUCCESS;
}


resource::e_error resource::file::flush_disk(sysapi::file::handle_t& hsock)
{
  return E_NOT_IMPL;
}

resource::e_error resource::file::flush_environ()
{
  return E_NOT_IMPL;
}

resource::e_error resource::file::flush_input(thr::pool::slot_t& thr_slot, buffer&)
{
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
}

resource::file::~file()
{
  if (opened == true)
    sysapi::file::close(file_handle);
}
