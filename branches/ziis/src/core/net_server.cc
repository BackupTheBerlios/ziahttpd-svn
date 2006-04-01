//
// net_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 16:50:30 2006 texane
// Last update Sat Apr 01 17:19:51 2006 texane
//


#include <list>
#include <ziafs_thr.hh>
#include <ziafs_core.hh>
#include <ziafs_net.hh>
#include <ziafs_config.hh>
#include <sys/sysapi.hh>
#include <iostream>


using namespace std;
using std::list;
using namespace sysapi;


// Exported

net::server::server(core_t* c)
{
  core = c;
  is_bound = false;
  srv_config = 0;
  srv_sock = 0;
  nr_bklog = 0;
}

bool net::server::reload_config(config* config)
{
  list<net::config::server*>::iterator curr_srv;
  unsigned short port;

  // destroy the current conf
  config->get_server(curr_srv);
  srv_config = config;
  nr_bklog = (*curr_srv)->nr_bklog;
  port = (*curr_srv)->port;
  insock::n_to_inaddr(srv_addr, INADDR_ANY, port);

  // build the module list
  m_modman.reload(config);

  return true;
}

bool net::server::run()
{
  // This function should
  // pass the server(this)
  // as a uparam and then
  // assign a new server_entry
  // task to the pool

  core->thr_pool->assign_task(thr::pool::server_entry, this);

  return true;
}
