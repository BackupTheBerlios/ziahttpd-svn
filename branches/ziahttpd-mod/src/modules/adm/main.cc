//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Mon Nov 14 16:19:53 2005 
//


#include <zia.hh>
#include <string>
#include <iostream>


using std::cout;
using std::cerr;
using std::endl;
using std::string;


// List of exported functions
MOD_EXPORT( HK_CREATE_CONNECTION )(http::session&, server::core*, int&);


// Exported function definitions


MOD_EXPORT( HK_CREATE_CONNECTION )(http::session& session, server::core* core, int&)
{
  cout << "\t[ * ] Administration module called" << endl;
  if (core)
    session.services_->echo("MESSAGE FROM ADMINISTRATION MODULE");
  cout << endl;

  return true;
}