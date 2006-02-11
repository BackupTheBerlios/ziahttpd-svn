//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Dec 06 12:05:17 2005 texane
// Last update Sun Feb 05 13:42:15 2006 texane
//


#include <iostream>
#include <sysapi/sysapi.hh>
#include <zt.hh>


using std::cout;
using std::cerr;
using std::endl;


// ?
// This program can flood your own machine;
// In order to avoid it, try to change the
// time wait value in the registry.
// -> HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\tcpip\Parameters
// Add the TcpTimedWaitDelay(REG_DWORD, ... value here, apparently should not
// be > 30 (decimal, in seconds)...)
// We are not on a server, so it's ok and the kernel will choose a socket
// name for us... but this is to take into account.

int main(int ac, char** av)
{
  zt::spool sp(ac, av);
  zt::client* cli;

  // Init the socket subsystem
  sysapi::socket_in::init_subsystem();

  // Start all test clients, and
  // wait for them to be done.
  while (sp.startNext(cli))
    ;
  while (sp.waitNext(cli))
    {
      if (cli)
	{
	  cli->report();
	  sp.handleEndofRequest(cli);
	}
    }

  // Release the socket subsystem
  sysapi::socket_in::release_subsystem();
}
