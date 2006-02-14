//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:04:58 2006 texane
// Last update Tue Feb 14 02:26:30 2006 texane
//


#include <iostream>
#include <windows.h>
#include <pthread.h>
#include <ziafs_thr.hh>


using std::endl;
using std::cout;


static void block_in_accept()
{
  cout << "[ ] do accept" << endl;
  Sleep(400);
  cout << "[x] do accept" << endl;
}

static void process_request()
{
  cout << "[ ] processing the request" << endl;
  Sleep(1000);
  cout << "[x] processing the request" << endl;
}

static void* server(thr::param_t& param)
{
  block_in_accept();
  param.thr_pool->assign_task(server, param);
  process_request();
  return 0;
}


int main(int, char** av)
{
  thr::pool pool(10);
  thr::param_t param;

  param.thr_pool = &pool;
  param.thr_slot = 0;

  while (1)
    pool.assign_task(server, param);
}
