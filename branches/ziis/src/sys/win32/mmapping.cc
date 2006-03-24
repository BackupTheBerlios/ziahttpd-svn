//
// mmapping.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Mar 24 18:17:24 2006 texane
// Last update Fri Mar 24 18:53:36 2006 texane
//


#include <string>
#include <sys/sysapi.hh>
#include <windows.h>


using std::string;


sysapi::error::handle_t sysapi::mmapping::alloc(handle_t& h_mmapping, const string& nm_file)
{
  bool is_success;
  HANDLE h_file;
  HANDLE h_map;
  BY_HANDLE_FILE_INFORMATION infos;
  unsigned char* p_map;

  sysapi::mmapping::reset(h_mmapping);

  is_success = true;
  h_file = INVALID_HANDLE_VALUE;
  h_map = INVALID_HANDLE_VALUE;
  
  // open the file
  h_file = CreateFile(nm_file.c_str(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
  if (h_file == INVALID_HANDLE_VALUE)
    {
      is_success = false;
      goto alloc_failed;
    }

  // get file information
  if (GetFileInformationByHandle(h_file, &infos) == FALSE)
    {
      is_success = false;
      goto alloc_failed;
    }

  // create the mmapping
  h_map = CreateFileMapping(h_file, 0, PAGE_READONLY, infos.nFileSizeHigh, infos.nFileSizeLow, 0);
  if (h_map == INVALID_HANDLE_VALUE)
    {
      is_success = false;
      goto alloc_failed;
    }

  // map view of file
  p_map = (unsigned char*)MapViewOfFile(h_map, FILE_MAP_READ, 0, 0, 0);
  if (p_map == 0)
    {
      is_success = false;
      goto alloc_failed;
    }
  
 alloc_failed:
  if (is_success == false)
    {
      if (p_map != 0)
	UnmapViewOfFile((LPVOID)p_map);
      if (h_map != INVALID_HANDLE_VALUE)
	CloseHandle(h_map);
      if (h_file != INVALID_HANDLE_VALUE)
	CloseHandle(h_file);
      return error::OPEN_FAILED;
    }
  else
    {
      h_mmapping.h_file = h_file;
      h_mmapping.h_mapping = h_map;
      h_mmapping.p_mmapping = p_map;
      h_mmapping.sz_mmapping = infos.nFileSizeLow;
    }
  
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::mmapping::release(handle_t& h_mmapping)
{
  if (h_mmapping.h_file != INVALID_HANDLE_VALUE)
    CloseHandle(h_mmapping.h_file);
  if (h_mmapping.h_mapping != INVALID_HANDLE_VALUE)
    CloseHandle(h_mmapping.h_mapping);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::mmapping::reset(handle_t& h_mmapping)
{
  h_mmapping.h_file = INVALID_HANDLE_VALUE;
  h_mmapping.h_mapping = INVALID_HANDLE_VALUE;
  h_mmapping.sz_mmapping = 0;
  h_mmapping.p_mmapping = 0;
  return error::SUCCESS;  
}


sysapi::error::handle_t sysapi::mmapping::size(handle_t& h_mmapping, unsigned long long& sz)
{
  sz = h_mmapping.sz_mmapping;
  return error::SUCCESS;
}
