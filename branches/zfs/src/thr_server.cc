//
// thr_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:37 2006 texane
// Last update Tue Feb 14 15:54:10 2006 texane
//


#include <cstdio>
#include <ziafs_debug.hh>
#include <ziafs_thr.hh>


static void block_in_accept()
{
  printf(">accepting\n");
}

static void process_request()
{
  printf(">processing request\n");
}

void* thr::server_entry(thr::pool::slot_t* thr_slot)
{
  block_in_accept();
  thr_slot->pool->assign_task(server_entry, thr_slot->uparam);
  process_request();
  return 0;
}
