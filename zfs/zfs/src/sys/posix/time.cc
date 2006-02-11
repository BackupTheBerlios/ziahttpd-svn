//
// time.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:51:16 2006 
// Last update Sat Feb 11 18:36:51 2006 
//


#include <sys/time.h>
#include <sys/sysapi.hh>


sysapi::error::handle_t sysapi::time::current(handle_t& htime)
{
  struct timeval tm_current;

  gettimeofday(&tm_current, 0);
  htime = tm_current.tv_sec * 1000000 + tm_current.tv_usec;
  return error::SUCCESS;
}
