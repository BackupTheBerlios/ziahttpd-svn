//
// resource_byfly.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 10:45:06 2006 texane
// Last update Wed Mar 22 23:29:38 2006 texane
//


#include <cstdio>
#include <iostream>
#include <sys/sysapi.hh>
#include <ziis.hh>
#include "include/constants.hh"
#include "include/resource.hh"


using namespace std;
using namespace sysapi;


// Internal routines

#define BODY	"<html><body><b>%s:<%u></b></body></html>"
#define STATUS	"ZIAFS_GENERATED_PAGE"
static void inline mk_status_msg(unsigned int err_code, unsigned char* buf, unsigned int& nbytes)
{
  nbytes = sprintf((char*)buf + nbytes, BODY, STATUS, err_code);
}


// resource interface implementation

resource::e_error resource::byfly::generate(unsigned int& nbytes)
{
  if (generated == true)
    return E_ALREADY_GEN;
  generated = true;

  data.resize(constants::BYFLY_SIZE);
  mk_status_msg(err_code, data.bufptr(), nbytes);
  data.resize(nbytes);
  return E_SUCCESS;
}

resource::e_error resource::byfly::flush_network(IOutput& out)
{
  int nr_sent;

  nr_sent = out.SendBuffer((const char*)data.bufptr(), (int)data.size());
  if (nr_sent < 0)
    return E_OP_ERROR;
  data.remove_front(nr_sent);
  return E_SUCCESS;
}

resource::e_error resource::byfly::flush_disk(sysapi::file::handle_t& hfile)
{
  return E_NOT_IMPL;
}

resource::e_error resource::byfly::flush_environ()
{
  return E_NOT_IMPL;
}

resource::e_error resource::byfly::flush_input(buffer&)
{
  return E_SUCCESS;
}

resource::e_error resource::byfly::size(unsigned int& nbytes)
{
  nbytes = (unsigned int)data.size();
  return E_SUCCESS;
}

bool resource::byfly::is_content_dynamic() const
{
  return false;
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
