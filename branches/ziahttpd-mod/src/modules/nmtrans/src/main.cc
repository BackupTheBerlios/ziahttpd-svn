//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Mon Nov 21 16:39:00 2005 texane
//


#include <zia.hh>
#include <string>
#include <iostream>


using std::cout;
using std::cerr;
using std::endl;
using std::string;


// List of exported functions

MOD_EXPORT( HK_ALTER_RQST_DATA )(http::session&, server::core*, int&);
MOD_EXPORT( HK_BUILD_RESP_METADATA )(http::session&, server::core*, int&);


// Exported function definitions

MOD_EXPORT( HK_ALTER_RQST_DATA )(http::session& session, server::core*, int&)
{
  cout << "\t[ * ] in the name translation module" << endl;
  cout << "\t[ * ] altering uri, wide name is now: " << session.uri().widename() << endl;

  if (sysapi::file::is_directory(session.uri().localname().c_str()))
    {
      // This is a directory, add backslash to the end of name
      if (session.uri().localname()[session.uri().localname().size() - 1] != '/')
	{
	  session.uri().widename() += '/';
	  session.uri().status() = 301;
	}
    }
  
  return true;
}


MOD_EXPORT( HK_BUILD_RESP_METADATA )(http::session& session, server::core* core, int&)
{
  if (session.uri().status() == 301)
    {
      session.info_out()["Location"] = "http://" + session.uri().widename();
    }
  return true;
}
