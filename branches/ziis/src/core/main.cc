//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:04:58 2006 texane
// Last update Mon Apr 03 19:33:15 2006 texane
//


#include <list>
#include <ziafs.hh>
#include <iostream>


using namespace std;


int main(int ac, char** av)
{
  core_t core;
  net::server* srv;
  net::config* curr_config;
  list<net::config::server*>::iterator curr_server;

  // Bootstrap the core
  sysapi::insock::init_subsystem();
  core.config = new net::config(ac == 2 ? av[1] : "conf/ziafs.xml");
//   else
//     core.config = new net::config("conf/ziafs.xml");
  core.thr_pool = new thr::pool(core.config->get_system()->nb_thread);
//   core.thr_pool->assign_task(thr::pool::system_entry, (void*)&core);

  // Create the servers
  core.config->get_server(curr_server);
  while (core.config->end_server(curr_server) == false)
    {
      // Build a new server
      srv = new net::server(&core);
      curr_config = new net::config;
      curr_config->generate_config(*curr_server, *core.config);
      srv->reload_config(curr_config);
      // release the server config list
      // delete curr_config;
      core.srv_list.push_front(srv);
      srv->run();
      ++curr_server;
    }

  // Main loop
  while (1)
    Sleep(999999);

  // Release (?)
  delete core.config;
  delete core.thr_pool;
  sysapi::insock::release_subsystem();
}
