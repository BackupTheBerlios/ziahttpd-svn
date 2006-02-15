//
// thr_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:37 2006 texane
// Last update Wed Feb 15 03:47:28 2006 
//


#include <cstdio>
#include <ziafs.hh>


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

  // Here handle the request
  Sleep(1000);

  // Handle the client session
  insock::close(cli_sock);

  return 0;
}
