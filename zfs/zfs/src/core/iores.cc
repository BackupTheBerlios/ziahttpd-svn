//
// resource.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 01:20:15 2006 texane
// Last update Sun Jan 22 13:53:00 2006 texane
//


#include <core/ziafs_io.hh>
#include <core/ziafs_status.hh>
#include <core/ziafs_buffer.hh>


using namespace io;
using namespace status;


resource::resource()
{
  m_pending = IO_NONE;
  m_state = ST_NONE;
}


error resource::fetch(buffer&)
{
  ziafs_return_status( NOTIMPL );
}


error resource::feed(buffer&)
{
  ziafs_return_status( NOTIMPL );
}