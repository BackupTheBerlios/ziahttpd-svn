//
// resource_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:15:23 2006 texane
// Last update Sat Feb 18 02:30:44 2006 texane
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
  e_error eerr;
  error::handle_t herr;
  unsigned int nsent;
  unsigned int nbytes;
  bool done;

  done = false;
  eerr = E_SUCCESS;

  // There are data in the buffer to be sent
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

  // Send the file
  if (eerr == E_SUCCESS)
    {
      if (TransmitFile(hsock, file_handle,
		       file_size, 0,
		       0, 0, TF_USE_DEFAULT_WORKER) == FALSE)
	eerr = E_OP_ERROR;
    }
  return eerr;
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
