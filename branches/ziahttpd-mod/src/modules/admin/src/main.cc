//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Wed Nov 16 16:18:30 2005 
//


// ?
// This module handles administration facilities, allowing for
// remote command line administration over http:
// . LOAD VERSION uri
// . UNLOAD HTTP_VERSION uri
// . DEBUG HTTP_VERSION


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


MOD_EXPORT( HK_PARSE_RQST_METADATA )(http::session& session, server::core* core, int&)
{

  // ?
  // The module must be loaded after http parser in the zia
  // configuration file (actually it needs the status line to be parsed).

  // if (session.msgdata... == LOAD | UNLOAD | DEBUG)

  cout << "[ + <Administration module>] " << endl;

  return true;
}
