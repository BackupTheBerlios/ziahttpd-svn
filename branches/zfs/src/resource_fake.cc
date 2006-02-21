//
// resource_fake.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 21 21:46:50 2006 texane
// Last update Tue Feb 21 22:07:21 2006 texane
//


// Implement the fake resource
// This resource has no content,
// that's used for the head request.


#include <ziafs_thr.hh>
#include <ziafs_resource.hh>
#include <ziafs_buffer.hh>
#include <sys/sysapi.hh>


using namespace sysapi;


resource::e_error resource::fake::generate(unsigned int& size)
{
  size = 0;
  if (generated == true)
    return E_ALREADY_GEN;
  generated = true;
  return E_SUCCESS;
}


resource::e_error resource::fake::flush_network(thr::pool::slot_t&, insock::handle_t&)
{
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


resource::e_error resource::fake::flush_input(thr::pool::slot_t&, buffer&)
{
  return E_SUCCESS;
}


resource::e_error resource::fake::size(unsigned int&)
{
  return E_SUCCESS;
}


// Construction/destruction

resource::fake::fake()
{
  generated = false;
}


resource::fake::~fake()
{
}
