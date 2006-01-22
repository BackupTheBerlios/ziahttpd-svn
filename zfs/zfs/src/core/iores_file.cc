//
// iores_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 03:10:24 2006 texane
// Last update Sun Jan 22 13:53:08 2006 texane
//


// Implement the file resource type


#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_status.hh>


using std::string;
using namespace io;
using namespace status;


error res_file::io_on_open()
{
  ziafs_return_status(NOTIMPL);
}

error res_file::io_on_close()
{
  ziafs_return_status(NOTIMPL);
}

error res_file::io_on_read()
{
  ziafs_return_status(NOTIMPL);
}

error res_file::io_on_write()
{
  ziafs_return_status(NOTIMPL);
}

error res_file::io_on_expire()
{
  ziafs_return_status(NOTIMPL);
}

error res_file::io_has_expired(bool&) const
{
  ziafs_return_status(NOTIMPL);
}

error res_file::dump(buffer&) const
{
  ziafs_return_status(NOTIMPL);
}
