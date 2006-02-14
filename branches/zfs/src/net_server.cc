//
// net_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 16:50:30 2006 texane
// Last update Tue Feb 14 17:30:20 2006 texane
//


#include <ziafs_thr.hh>
#include <ziafs_core.hh>
#include <ziafs_net.hh>


// Exported

net::server::server(core_t* c)
{
  core = c;
}

bool net::server::reload_config()
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

  core->thr_pool->assign_task(thr::pool::server_entry, 0);

  return true;
}
