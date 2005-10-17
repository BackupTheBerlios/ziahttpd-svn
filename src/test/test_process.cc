//
// test_process.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Mon Oct 10 18:42:26 2005 texane
// Last update Tue Oct 11 11:18:40 2005 texane
//


#include <iostream>
#include <sysapi.hh>


// This test file implements a ps-like tool
// by creating two cat processes

const char* av[] =
  {
    "tmp/cat.exe",
    0
  };

static void processes_list()
{
  sysapi::process::handle_t myhdl;

  if (sysapi::process::myhandle(&myhdl) == false)
    {
      sysapi::error::stringify("Cannot get my handle:");
      return ;
    }

  std::cout << "Got my handle\n";
}

void	test_process()
{
  std::cout << "Testing process sysapi start\n";

  // Test the creation of a process
  {
    sysapi::process::handle_t phdl1;
    sysapi::process::handle_t phdl2;

    // create two processes
    if (sysapi::process::create_and_loadexec(&phdl1, sizeof(av) / sizeof(char*) - 1, av) == false)
      {
	sysapi::error::stringify("Cannot create process:");
	return ;
      }
    if (sysapi::process::create_and_loadexec(&phdl2, sizeof(av) / sizeof(char*) - 1, av) == false)
      {
	sysapi::error::stringify("Cannot create process:");
	sysapi::process::release(phdl1);
	return ;
      }

    std::cout << "Two processes created\n";

    // Wait on those two process to terminate
    sysapi::process::wait_single(phdl1);
    sysapi::process::wait_single(phdl2);

    std::cout << "Processes are now done\n";

    sysapi::process::release(phdl1);
    sysapi::process::release(phdl2);

    processes_list();
  }

  std::cout << "Testing process sysapi done\n";
}
