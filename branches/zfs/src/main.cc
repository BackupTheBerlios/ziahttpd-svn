//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:04:58 2006 texane
// Last update Tue Feb 14 17:27:02 2006 texane
//


#include <list>
#include <ziafs.hh>


int main(int, char**)
{
  core_t core;
  net::server* srv;

  // Bootstrap the core
  core.thr_pool = new thr::pool(50);
  core.thr_pool->assign_task(thr::pool::system_entry, 0);
  srv = new net::server(&core);
  srv->reload_config();
  srv->run();
  core.srv_list.push_front(srv);

  // Main loop
  while (1)
    Sleep(5000000);

  // Release (?)
  delete core.thr_pool;
}
