//
// thr_io.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 22:56:37 2006 texane
// Last update Wed Feb 15 04:25:08 2006 
//


#include <ziafs_thr.hh>
#include <sys/sysapi.hh>


void thr::io_info_reset(thr::io_info_t& io_info)
{
  io_info.in_progress = false;
  io_info.sz = 0;
  io_info.tm_start = 0;
}


sysapi::error::handle_t thr::recv(thr::pool::slot_t& slot, sysapi::insock::handle_t& hsock, unsigned char* buf, unsigned int len, unsigned int& nrecv)
{
  sysapi::error::handle_t herr;

  slot.curr_io.in_progress = true;
  herr = sysapi::insock::recv(hsock, buf, len, nrecv);
  slot.curr_io.in_progress = false;
  return herr;
}


sysapi::error::handle_t thr::send(thr::pool::slot_t& slot, sysapi::insock::handle_t& hsock, unsigned char* buf, unsigned int len, unsigned int& nsent)
{
  sysapi::error::handle_t herr;

  slot.curr_io.in_progress = true;
  herr = sysapi::insock::send(hsock, buf, len, nsent);
  slot.curr_io.in_progress = false;
  return herr;
}
