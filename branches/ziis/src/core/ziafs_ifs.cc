//
// ziafs_ifs.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Apr 04 14:58:50 2006 texane
// Last update Thu Apr  6 22:30:03 2006 
//


#include <ziafs.hh>
#include <cstring>
#include <sys/sysapi.hh>


int ZfsIfs::IFSOpen(const char* str_filename, const char* str_omode)
{
  sysapi::file::handle_t h_file;
  sysapi::error::handle_t h_err;
  int m_omode;

  h_err = sysapi::error::OPEN_FAILED;
  m_omode = (int)sysapi::file::O_READ;

  if (strcmp(str_omode, "r") == 0)
    {
      m_omode = (int)sysapi::file::O_READ;
    }
  else if (strcmp(str_omode, "r+") == 0)
    {
      m_omode = sysapi::file::O_READ | sysapi::file::O_WRITE;
    }
  else if (strcmp(str_omode, "w") == 0)
    {
      IFSRemove(str_filename);
      m_omode = sysapi::file::O_WRITE;
    }
  else if (strcmp(str_omode, "w+") == 0)
    {
      IFSRemove(str_filename);
      m_omode = sysapi::file::O_READ | sysapi::file::O_WRITE;
    }
  else if (strcmp(str_omode, "a") == 0)
    {
      m_omode = sysapi::file::O_WRITE;
    }
  else if (strcmp(str_omode, "a+") == 0)
    {
      m_omode = sysapi::file::O_READ | sysapi::file::O_WRITE;
    }
  h_err = sysapi::file::open(h_file, str_filename, (sysapi::file::omode_t)m_omode);
  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return (int)h_file;
}


int ZfsIfs::IFSRead(int id_file, long long off_file, char* p_buf, int ln_buf)
{
  sysapi::file::handle_t h_file;
  sysapi::error::handle_t h_err;
  unsigned int nr_read;

  h_err = sysapi::file::read(h_file, (unsigned char*)p_buf, (unsigned int)ln_buf, nr_read);
  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return (int)nr_read;
}


int ZfsIfs::IFSWrite(int id_file, const char* p_buf, int ln_buf)
{
  sysapi::file::handle_t h_file;
  sysapi::error::handle_t h_err;
  unsigned int nr_written;

  h_err = sysapi::file::write(h_file, (unsigned char*)p_buf, (unsigned int)ln_buf, nr_written);
  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return (int)nr_written;
}


void ZfsIfs::IFSClose(int id_file)
{
  sysapi::file::close((sysapi::file::handle_t&)id_file);
}


long long ZfsIfs::IFSGetSize(int id_file)
{
  sysapi::file::handle_t h_file;
  unsigned long long sz_file;

  h_file = (sysapi::file::handle_t)id_file;
  sz_file = 0;
  sysapi::file::size(h_file, sz_file);
  return (long long)sz_file;
}


bool ZfsIfs::IFSRemove(const char* nm_file)
{
  sysapi::error::handle_t h_err;
  
  h_err = sysapi::file::remove(nm_file);
  if (h_err != sysapi::error::SUCCESS)
    return false;
  return true;
}
