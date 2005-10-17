//
// core.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Oct 11 21:28:14 2005 texane
// Last update Wed Oct 12 18:38:06 2005 texane
//


// Implementation of server core


#include <iostream>
#include <core.hh>
#include <sysapi.hh>


// - Construction, destruction

server::core::core()
{
  done_ = false;
  initialized_ = false;

  // initialize the socket subsystem
  if (sysapi::socket_in::init_subsystem() == false)
    {
      sysapi::error::stringify();
    }

  config_init();
  std::cout << "Welcome in the server core\n";
}


server::core::~core()
{
  sysapi::socket_in::release_subsystem();
  std::cout << "Server core has just been released\n";
}


bool server::core::config_init()
{
  if (initialized_ == true)
    {
      config_reset();
    }
  // ...
  return false;
}

bool server::core::config_reset()
{
  if (initialized_ == true)
    {
      initialized_ = false;
      // ...
    }
  return false;
}
