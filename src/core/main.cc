//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 05 11:32:11 2005 texane
// Last update Mon Oct 17 13:25:44 2005 texane
//


#include <iostream>
#include <core.hh>
#include <sysapi.hh>


// Create a session spool


int main(int ac, char** av)
{
  server::core srv;
  sysapi::thread::handle_t hdl_thr;

  while (srv.done() == false)
    {
      if (sysapi::thread::create_and_exec(&hdl_thr, sysapi::thread::SUSPENDED, server::session::entrypoint_) == false)
	{
	  sysapi::error::stringify("Cannot create thread: ");
	  return -1;
	}
      sysapi::thread::signal(hdl_thr, sysapi::thread::RESUME);
      sysapi::thread::wait_single(hdl_thr);
    }

  std::cout << std::endl;
}
