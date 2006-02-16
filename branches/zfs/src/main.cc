//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:04:58 2006 texane
// Last update Thu Feb 16 12:29:35 2006 texane
//


#include <list>
#include <ziafs.hh>


using std::list;


int main(int, char**)
{
  core_t core;
  net::server* srv;
  net::config* curr_config;
  list<net::config::server*>::iterator curr_server;

  // Bootstrap the core
  sysapi::insock::init_subsystem();
  core.config = new net::config("ziafs.xml");
  core.thr_pool = new thr::pool(core.config->get_system()->nb_thread);
  core.thr_pool->assign_task(thr::pool::system_entry, 0);

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
      delete curr_config;
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
