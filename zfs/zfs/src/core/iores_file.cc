//
// iores_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 03:10:24 2006 texane
// Last update Sun Jan 29 15:58:33 2006 texane
//


// Implement the file resource type


#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_io.hh>
#include <core/ziafs_status.hh>


using std::string;
using namespace io;
using namespace status;


// construction/destruction
res_file::res_file(stmask openmod, const string& path) : resource()
{
  m_path = path;
  m_filesz = 0;
  m_nrtoread = 0;
}

res_file::~res_file()
{
}


// resource interface implementation

error res_file::io_on_open()
{
//   sysapi::error::handle_t herr;

//   if (openmode)

//   herr = sysapi::file::open(hfile, "toto");
//   if (sysapi::error::is_success(herr) == true)
//     ziafs_return_status(SUCCESS);

  ziafs_return_status(NOTIMPL);
}

error res_file::io_on_close(void*&)
{
  ziafs_return_status(NOTIMPL);
}

error res_file::io_on_read(void*&, void*&)
{
  ziafs_return_status(NOTIMPL);
}

error res_file::io_on_write(void*&, void*&)
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

error res_file::name(string& name) const
{
  name = m_path;
  ziafs_return_status(SUCCESS);
}
