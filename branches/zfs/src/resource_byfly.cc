//
// resource_byfly.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:16:34 2006 texane
// Last update Fri Feb 17 14:52:59 2006 texane
//


#include <cstdio>
#include <iostream>
#include <sys/sysapi.hh>
#include <ziafs_static.hh>
#include <ziafs_resource.hh>


using namespace sysapi;


// Internal routines

#define BODY	"<html><body><b>%s</b></body></html>"
#define STATUS	"ZIAFS_GENERATED_PAGE"
#define HEADER	"http/1.1 200 OK\r\ncontent-length: %d\r\n\r\n", strlen(BODY)
static void inline mk_response(unsigned char* buf, unsigned int& nbytes)
{
  nbytes = 0;
  nbytes += sprintf((char*)buf, HEADER);
  nbytes += sprintf((char*)buf + nbytes, BODY, STATUS);
}


// resource interface implementation

resource::e_error resource::byfly::generate(unsigned int& nbytes)
{
  data.resize(ZIAFS_STATIC_BUFSZ);
  mk_response(data.bufptr(), nbytes);
  data.resize(nbytes);
  return E_SUCCESS;
}

resource::e_error resource::byfly::flush_network(thr::pool::slot_t& thr_slot, insock::handle_t& hsock)
{
  error::handle_t herr;
  unsigned int nsent;

  herr = send(thr_slot, hsock, data.bufptr(), (unsigned int)data.size(), nsent);
  if (thr_slot.curr_io.timeouted == true)
    return E_OP_ERROR;
  else if (herr != error::SUCCESS)
    return E_OP_ERROR;
  return E_SUCCESS;
}

resource::e_error resource::byfly::flush_disk(file::handle_t& hsock)
{
  return E_NOT_IMPL;
}

resource::e_error resource::byfly::flush_environ()
{
  return E_NOT_IMPL;
}

resource::e_error resource::byfly::flush_input()
{
  return E_NOT_IMPL;
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
}

resource::byfly::~byfly()
{
  
}
