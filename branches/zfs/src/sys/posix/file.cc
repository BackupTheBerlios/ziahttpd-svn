//
// file.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:01:40 2006 
// Last update Sun Feb 12 09:03:34 2006 texane
//

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/sysapi.hh>


using std::string;


sysapi::error::handle_t sysapi::file::open(handle_t& hfile, const std::string& path, omode_t omode)
{
  int fd;
  int op_mod;

  hfile = -1;
  op_mod = 0;
  if (omode == O_BOTH)
    op_mod = O_RDWR;
  else if (omode == O_READ)
    op_mod = O_RDONLY;
  else if (omode == O_WRITE)
    op_mod = O_WRONLY;
  fd = ::open(path.c_str(), op_mod);
  if (fd == -1)
    return error::OPEN_FAILED;
  hfile = fd;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::close(handle_t& hfile)
{
  close(hfile);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::read(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nread)
{
  int nret;

  nret = ::read(hfile, buf, nbytes);
  if (nret == -1)
    return error::READ_FAILED;
  nread = (unsigned int)nret;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::write(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nwritten)
{
  int nret;

  nret = ::write(hfile, buf, nbytes);
  if (nret == -1)
    return error::WRITE_FAILED;
  nwritten = (unsigned int)nret;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::size(handle_t& hfile, unsigned long long& sz)
{
  struct stat st;

  if (fstat(hfile, &st) == -1)
    return error::UNKNOWN;
  sz = (unsigned long long)st.st_size;
  return error::SUCCESS;
}
