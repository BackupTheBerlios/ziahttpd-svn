//
// net_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 16:50:30 2006 texane
// Last update Wed Feb 15 04:06:21 2006 
//


#include <ziafs_thr.hh>
#include <ziafs_core.hh>
#include <ziafs_net.hh>
#include <ziafs_config.hh>
#include <sys/sysapi.hh>


using namespace sysapi;


// Exported

net::server::server(core_t* c)
{
  core = c;
  is_bound = false;
  srv_sock = 0;
  nr_bklog = 10;
  //   insock::p_to_inaddr(srv_addr, "localhost", 40000);
  insock::n_to_inaddr(srv_addr, INADDR_ANY, 40000);
}

bool net::server::reload_config(config*)
{
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
