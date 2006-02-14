//
// thr_io.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 22:56:37 2006 texane
// Last update Tue Feb 14 23:07:56 2006 texane
//


#include <ziafs_thr.hh>
#include <sys/sysapi.hh>


void thr::io_info_reset(thr::io_info_t& io_info)
{
  io_info.in_progress = false;
  io_info.sz = 0;
  io_info.tm_start = 0;
}


int thr::recv(thr::pool::slot_t& slot, unsigned char*, unsigned int)
{
  slot.curr_io.in_progress = true;
  slot.curr_io.in_progress = false;
  return 0;
}


int thr::send(thr::pool::slot_t& slot, unsigned char*, unsigned int)
{
  slot.curr_io.in_progress = true;
  slot.curr_io.in_progress = false;
  return 0;
}
