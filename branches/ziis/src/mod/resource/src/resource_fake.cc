//
// resource_fake.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 16:44:14 2006 texane
// Last update Wed Mar 22 16:54:29 2006 texane
//


#include "include/mod_resource.hh"


using namespace sysapi;


resource::e_error resource::fake::generate(unsigned int& size)
{
  size = 0;
  if (generated == true)
    return E_ALREADY_GEN;
  generated = true;
  return E_SUCCESS;
}


resource::e_error resource::fake::flush_network(IOutput& out)
{
  int nr_sent;

  nr_sent = out.SendBuffer((const char*)data.bufptr(), (int)data.size());
  if (nr_sent < 0)
    return E_OP_ERROR;
  return E_SUCCESS;
}


resource::e_error resource::fake::flush_disk(sysapi::file::handle_t&)
{
  return E_SUCCESS;
}


resource::e_error resource::fake::flush_environ()
{
  return E_SUCCESS;
}


resource::e_error resource::fake::flush_input(buffer&)
{
  return E_SUCCESS;
}


resource::e_error resource::fake::size(unsigned int& nr_bytes)
{
  nr_bytes = 0;
  return E_SUCCESS;
}


bool resource::fake::is_content_dynamic() const
{
  return false;
}


// Construction/destruction

resource::fake::fake()
{
  generated = false;
}


resource::fake::~fake()
{
}

