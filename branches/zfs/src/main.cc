//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:04:58 2006 texane
// Last update Thu Feb 16 00:26:51 2006 
//


#include <list>
#include <ziafs.hh>


int main(int, char**)
{
  core_t core;
  net::server* srv;

  // Bootstrap the core
  sysapi::insock::init_subsystem();
  core.config = new net::config("ziafs.xml");
  core.thr_pool = new thr::pool(core.config->get_system()->nb_thread);
  core.thr_pool->assign_task(thr::pool::system_entry, 0);

  // Build a new server
  srv = new net::server(&core);
  srv->reload_config(core.config);
  core.srv_list.push_front(srv);
  srv->run();

  // Main loop
  while (1)
    Sleep(999999);

  // Release (?)
  delete core.config;
  delete core.thr_pool;
  sysapi::insock::release_subsystem();
}
