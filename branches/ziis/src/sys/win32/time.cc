//
// time.cc<2> for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:52:29 2006 
// Last update Sat Feb 11 17:54:03 2006 
//


#include <sys/sysapi.hh>


sysapi::error::handle_t sysapi::time::current(handle_t& htime)
{
  SYSTEMTIME tm_now;
  FILETIME fltm_now;
  ULARGE_INTEGER ul_now;

  htime = 0;
  GetSystemTime(&tm_now);
  SystemTimeToFileTime(&tm_now, &fltm_now);
  memcpy((void*)&ul_now, (const void*)&fltm_now, sizeof(unsigned long long));
  htime = ul_now.QuadPart;

  return error::SUCCESS;
}
