//
// thr_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:37 2006 texane
// Last update Tue Feb 14 16:31:42 2006 texane
//


#include <cstdio>
#include <ziafs_debug.hh>
#include <ziafs_thr.hh>
#include <windows.h>


static void block_in_accept()
{
  printf(">accepting\n");
  Sleep(1000);
}

static void process_request()
{
  printf(">processing request\n");
  Sleep(1000);
}

void* thr::pool::server_entry(thr::pool::slot_t* thr_slot)
{
  block_in_accept();
  thr_slot->pool->assign_task(server_entry, thr_slot->uparam);
  process_request();
  return 0;
}
