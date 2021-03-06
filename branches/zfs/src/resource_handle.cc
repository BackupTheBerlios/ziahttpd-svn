//
// resource_handle.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:21:12 2006 texane
// Last update Wed Feb 22 15:41:46 2006 texane
//


#include <ziafs_buffer.hh>
#include <ziafs_resource.hh>


// either the recopy constructor is not ok
// either the hdr_buffer is not ok
// either the + operator  is enot ok


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


unsigned int resource::handle::input_size()
{
  return in_size;
}


bool resource::handle::is_prefetched_input()
{
  return in_buf.size() ? true : false;
}


void resource::handle::get_prefetched_input(buffer& data)
{
  data = in_buf;
  in_buf.clear();
}


resource::handle::handle()
{
}
