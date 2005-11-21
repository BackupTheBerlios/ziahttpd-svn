//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Mon Nov 21 15:57:29 2005 texane
//


#include <zia.hh>
#include <string>
#include <iostream>


using std::cout;
using std::cerr;
using std::endl;
using std::string;


// List of exported functions
MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session&, server::core*, int&);


// Exported function definitions


MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session& session, server::core*, int&)
{
  if (session.info_in().method_string() == "get")
    {
      cout << "\t[ * ] CGI MODULE CALLED with method <GET>" << endl;
      cout << "uri      : " << session.uri().widename() << endl;
      cout << "localname: " << session.uri().localname() << endl;
      cout << "extension: " << session.uri().extension() << endl;
    }
  else
    {
      cout << "\t[ * ] CGI MODULE CALLED denying the request to be processed" << endl;
    }

  return true;
}
