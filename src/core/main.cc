//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 05 11:32:11 2005 texane
// Last update Thu Oct 20 21:21:58 2005 
//


#include <iostream>
#include <server.hh>


int main(int ac, char** av)
{
  // Init the socket subsystem
  if (sysapi::socket_in::init_subsystem() == false)
    {
      sysapi::error::stringify("Cannot initialize subsystem: ");
      return -1;
    }

  // Create a new channel
  {
    server::channel chan("40000");
    sysapi::thread::wait_single(chan.dispatcher_handle());
    sysapi::thread::release(chan.dispatcher_handle());
  }

  // Release the subsystem
  sysapi::socket_in::release_subsystem();
}
