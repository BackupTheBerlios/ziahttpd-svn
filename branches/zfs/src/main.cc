//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:04:58 2006 texane
// Last update Wed Feb 15 00:58:49 2006 texane
//


#include <list>
#include <ziafs.hh>


int main(int, char**)
{
  core_t core;
  net::server* srv;

  // Bootstrap the core
  sysapi::insock::init_subsystem();
//   core.thr_pool = new thr::pool(50);
  core.thr_pool = new thr::pool(3);
  core.thr_pool->assign_task(thr::pool::system_entry, 0);
  srv = new net::server(&core);
  srv->reload_config();
  core.srv_list.push_front(srv);
  srv->run();

  // Main loop
  while (1)
    Sleep(5000000);

  // Release (?)
  delete core.thr_pool;
  sysapi::insock::release_subsystem();
}
