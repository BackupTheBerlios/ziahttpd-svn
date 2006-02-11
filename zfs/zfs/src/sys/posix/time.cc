//
// time.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:51:16 2006 
// Last update Sat Feb 11 17:52:23 2006 
//


#include <sys/time.h>
#include <sys/sysapi.hh>


sysapi::error::handle_t sysapi::time::current(handle_t& htime)
{
  htime = 0;
  return error::SUCCESS;
}
