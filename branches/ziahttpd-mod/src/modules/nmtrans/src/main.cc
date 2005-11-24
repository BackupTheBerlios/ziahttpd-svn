//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Wed Nov 23 11:57:50 2005 texane
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
//bool	directoryindex(http::session& session);

// Exported function definitions

MOD_EXPORT( HK_ALTER_RQST_DATA )(http::session& session, server::core*, int&)
{
  //std::cout << "doc root : " << session.conf().GetSimpleString("documentroot") << std::endl;
  session.uri().localname() = session.conf().GetSimpleString("documentroot") + session.uri().widename();
session.uri().localname() = "../root/www/" + session.uri().widename();
  if (sysapi::file::is_directory(session.uri().localname().c_str())
      && session.uri().localname()[session.uri().localname().size() - 1] != '/')
    {
      // This is a directory, add backslash to the end of name
      session.uri().widename() += '/';
      session.uri().status() = 301;
    }


	// translate name



	//if (session.uri().localname()[session.uri().localname().size() - 1] == '/')
	//{
	//	// if the wwwname finish by a /, check Directory index
	//	if (!directoryindex(session))
	//	{
	//		//call directory listing cgi
	//	}

	//}
	std::cout << "localfile :" << session.uri().localname() << std::endl;
//	session.uri().localname() = "cgi-get-windows.exe";

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

//bool	directoryindex(http::session& session)
//{
//	char	*dir[] = {"", "", 0};
//	return (true);
//}