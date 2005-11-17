//
// uri.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:51:23 2005 
// Last update Sun Nov 13 17:44:26 2005 
//


#include <string>
#include <http/session.hh>
#include <http/uri.hh>


using std::string;


http::uri::uri()
{
	status_ = 200;
}


http::uri::uri(session& s)
{

}


int&	http::uri::status()
{
  return status_;
}

string&	http::uri::strstatus()
{
  return strstatus_;
}



string&	http::uri::localname()
{
  return localname_;
}


string&	http::uri::widename()
{
  return widename_;
}


string&	http::uri::extension()
{
  return extension_;
}
