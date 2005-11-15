//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Mon Nov 14 16:19:23 2005 
//


#include <zia.hh>
#include <iostream>


using std::cout;
using std::cerr;
using std::endl;


// List of exported functions
MOD_EXPORT( HK_CREATE_CONNECTION )(http::session&, server::core*, int&);
MOD_EXPORT( HK_RELEASE_CONNECTION )(http::session&, server::core*, int&);


// Exported function definitions

MOD_EXPORT( HK_CREATE_CONNECTION )(http::session& session, server::core*, int&)
{
  cout << "\t -> Creating connection from module" << endl;
  session.services_->echo("test module");
  return true;
}


MOD_EXPORT( HK_RELEASE_CONNECTION )(http::session&, server::core*, int&)
{
  cout << "\t -> Releaseing connection from module" << endl;
  return true;
}
