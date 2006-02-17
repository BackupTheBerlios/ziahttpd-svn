//
// file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 14:10:39 2006 texane
// Last update Fri Feb 17 16:55:06 2006 texane
//


#include <string>
#include <sys/sysapi.hh>
#include <windows.h>


using std::string;


sysapi::error::handle_t sysapi::file::open(handle_t& hfile, const std::string& path, omode_t omode)
{
  sysapi::error::handle_t herr;
  DWORD shmode;
  DWORD op_mod;

  herr = error::SUCCESS;
  shmode = 0;
  op_mod = 0;
  if (omode & O_READ)
    {
      op_mod |= GENERIC_READ;
      shmode |= FILE_SHARE_READ;
    }
  if (omode & O_WRITE)
    {
      op_mod |= GENERIC_WRITE;
      shmode |= FILE_SHARE_WRITE;
    }
  hfile = CreateFile(path.c_str(), op_mod, shmode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hfile == INVALID_HANDLE_VALUE)
    herr = error::OPEN_FAILED;
    // herr = GetLastError();
  return herr;
}


sysapi::error::handle_t sysapi::file::close(handle_t& hfile)
{
  CloseHandle(hfile);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::read(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nread)
{
  DWORD nr_bytes;
  BOOL ret;

  nread = 0;
  ret = ReadFile(hfile, static_cast<LPVOID>(buf), nbytes, &nr_bytes, NULL);
  if (ret == FALSE)
    return error::READ_FAILED;
  nread = (unsigned int)nr_bytes;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::write(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nwritten)
{
  DWORD nr_bytes;
  BOOL ret;

  ret = WriteFile(hfile, reinterpret_cast<LPVOID>((void*)buf), nbytes, &nr_bytes, NULL);
  if (ret == FALSE)
    return error::WRITE_FAILED;

  nwritten = nr_bytes;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::size(handle_t& hfile, unsigned long long& sz)
{
  BY_HANDLE_FILE_INFORMATION info;

  if (GetFileInformationByHandle(hfile, &info) == FALSE)
    return error::UNKNOWN;
  sz = info.nFileSizeLow;
  return error::SUCCESS;
}
