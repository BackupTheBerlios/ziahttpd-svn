//
// thr_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:37 2006 texane
// Last update Tue Feb 14 18:36:19 2006 texane
//


#include <cstdio>
#include <ziafs_thr.hh>
#include <ziafs_net.hh>
#include <sys/sysapi.hh>
#include <ziafs_debug.hh>


using namespace sysapi;


void* thr::pool::server_entry(thr::pool::slot_t* thr_slot)
{
  // Server's main function
  // thr_slot->uparam contains
  // the server instance.
  // If the server is not yet
  // bound, bind it.

  net::server* srv = (net::server*)thr_slot->uparam;
  insock::handle_t cli_sock;
  struct sockaddr_in cli_addr;

  // The sever is not yet bound
  srv = (net::server*)thr_slot->uparam;
  if (srv->is_bound == false)
    {
      srv->is_bound = true;
      insock::create_listening(srv->srv_sock, srv->srv_addr, srv->nr_bklog);
    }

  // Accept a new connection, delegate accept
  insock::accept(cli_sock, cli_addr, srv->srv_sock);
  thr_slot->pool->assign_task(server_entry, (void*)srv);

  // Handle the client
  printf("client is now connected\n");
  Sleep(100000);
//   insock::close(cli_sock);

  return 0;
}
