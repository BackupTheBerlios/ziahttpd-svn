//
// resource_byfly.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:16:34 2006 texane
// Last update Fri Feb 24 00:18:33 2006 
//


#include <cstdio>
#include <iostream>
#include <sys/sysapi.hh>
#include <ziafs_static.hh>
#include <ziafs_resource.hh>


using namespace sysapi;


// Internal routines

#define BODY	"<html><body><b>%s:<%u></b></body></html>"
#define STATUS	"ZIAFS_GENERATED_PAGE"
static void inline mk_status_msg(unsigned int err_code, unsigned char* buf, unsigned int& nbytes)
{
  nbytes = 0;
  nbytes += sprintf((char*)buf + nbytes, BODY, STATUS, err_code);
}


// resource interface implementation

resource::e_error resource::byfly::generate(unsigned int& nbytes)
{
  if (generated == true)
    return E_ALREADY_GEN;
  generated = true;

  data.resize(ZIAFS_STATIC_BYFLYSZ);
  mk_status_msg(err_code, data.bufptr(), nbytes);
  data.resize(nbytes);
  return E_SUCCESS;
}

resource::e_error resource::byfly::flush_network(thr::pool::slot_t& thr_slot, insock::handle_t& hsock)
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

resource::e_error resource::byfly::flush_disk(sysapi::file::handle_t& hfile)
{
  return E_NOT_IMPL;
}

resource::e_error resource::byfly::flush_environ()
{
  return E_NOT_IMPL;
}

resource::e_error resource::byfly::flush_input(thr::pool::slot_t& thr_slot, buffer&)
{
  return E_SUCCESS;
}

resource::e_error resource::byfly::size(unsigned int& nbytes)
{
  nbytes = 0;
  return E_NOT_IMPL;
}


// construction/destruction

resource::byfly::byfly(unsigned int e_code)
{
  err_code = e_code;
  generated = false;
}

resource::byfly::~byfly()
{
  
}
