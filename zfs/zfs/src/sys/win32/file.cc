//
// file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 14:10:39 2006 texane
// Last update Sun Jan 22 15:37:22 2006 texane
//


#include <string>
#include <windows.h>
#include <sys/sysapi.hh>


using std::string;


sysapi::error::handle_t sysapi::file::open(handle_t& hfile, const std::string& path)
{
  error::handle_t herr;
  DWORD shmode;

//   herr = SUCCESS;
  shmode = FILE_SHARE_WRITE;
  hfile = CreateFile(path.c_str(), 0, shmode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hfile == INVALID_HANDLE_VALUE)
    herr = GetLastError();
  return herr;
}


sysapi::error::handle_t sysapi::file::close(handle_t& hfile)
{
  CloseHandle(hfile);

  return 0;
//   return SUCCESS;
}