//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 05 11:32:11 2005 texane
// Last update Wed Oct 19 15:40:39 2005 texane
//


#include <iostream>
#include <server.hh>

// Currently the server supports only
// one channel (INADDR_ANY, 40000).

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
    server::channel chan("40000", "localhost");
    sysapi::thread::wait_single(chan.dispatcher_handle());
    sysapi::thread::release(chan.dispatcher_handle());
  }

  // Release the subsystem
  sysapi::socket_in::release_subsystem();
}
