//
// client.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 08:26:20 2005 texane
// Last update Wed Oct 12 15:03:16 2005 texane
//


#include <iostream>
#include <client.hh>
#include <sysapi.hh>


using server::client;


// Construction, destruction
server::client::client()
{
//   thr_hdl_ = sysapi::thread::create();
}

server::client::~client()
{
  // close opened connections
  sysapi::thread::release(thr_hdl_);
}
