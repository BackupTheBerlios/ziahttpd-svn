//
// resource_handle.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:21:12 2006 texane
// Last update Sat Feb 18 12:41:33 2006 texane
//


#include <ziafs_buffer.hh>
#include <ziafs_resource.hh>


resource::e_error resource::handle::prepend_header(buffer& hdr_buf)
{
  data = hdr_buf + data;
  return E_SUCCESS;
}


resource::e_error resource::handle::alter_content(void (buffer&, void*), void*)
{
  return E_SUCCESS;
}


bool resource::handle::is_input() const
{
  if (omode == O_INPUT || omode == O_BOTH)
    return true;
  return false;
}


bool resource::handle::is_output() const
{
  if (omode == O_OUTPUT || omode == O_BOTH)
    return true;
  return false;
}


resource::handle::handle()
{
}
