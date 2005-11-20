//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Sun Nov 20 17:29:32 2005 texane
//


#include <server/core.hh>
#include <http/session.hh>
#include <server/modhelper.hh>
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
  session.services_->echo("salutr les amis comment ca roule");
  return true;
}
