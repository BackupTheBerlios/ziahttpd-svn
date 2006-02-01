//
// iores_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 03:10:24 2006 texane
// Last update Wed Feb 01 23:17:35 2006 texane
//


// Implement the file resource type


#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_io.hh>
#include <core/ziafs_debug.hh>
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
  // Think about reseting all
  // Windows specific code

  BY_HANDLE_FILE_INFORMATION info;
  sysapi::error::handle_t herr;

  herr = sysapi::file::open(m_hfile, m_path);
  if (herr != sysapi::error::SUCCESS)
    ziafs_return_status(status::CANNOT_OPEN);

  if (GetFileInformationByHandle(m_hfile, &info) == FALSE)
    ziafs_return_status(status::CANNOT_OPEN);

  m_filesz = (unsigned long)info.nFileSizeLow;
  m_nrtoread = (int)m_filesz;

  ziafs_return_status(status::CANNOT_OPEN);
}

error res_file::io_on_close(void*&)
{
  // Close all if reset
  // unmap view of file
  // close map handle
  sysapi::file::close(m_hfile);
  ziafs_return_status(status::SUCCESS);
}

error res_file::io_on_read(void*& pdata, void*& aux)
{
  sysapi::error::handle_t herr;
  unsigned int nread;
  net::server* srv;
  buffer* buf;
#define BUFSZ 512

  if (m_nrtoread <= 0)
    ziafs_return_status(status::CANNOT_READ);

  srv = (net::server*&)aux;
  buf = new buffer;
  buf->resize(BUFSZ);
  
  herr = sysapi::file::read(m_hfile, buf->bufptr(), BUFSZ, nread);
  if (herr != sysapi::error::SUCCESS)
    ziafs_return_status(status::CANNOT_READ);

  m_nrtoread -= nread;
  buf->resize(nread);
  m_rd_buf += *buf;
  delete buf;
  
  ziafs_return_status(status::SUCCESS);
}

error res_file::io_on_write(void*&, void*&)
{
  ziafs_return_status(SUCCESS);
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
