//
// service.cc for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:45:55 2005 
// Last update Mon Nov 14 15:48:09 2005 
//


#include <string>
#include <iostream>
#include <server/core.hh>
#include <server/service.hh>


using std::string;
using std::cout;
using std::endl;


void	server::service::echo(const string& what)
{
  cout << "[*] From the echo service: " << what << endl;
}
