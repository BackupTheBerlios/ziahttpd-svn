//
// msgdata.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:56:50 2005 
// Last update Sun Nov 13 16:05:39 2005 
//


#include <string>
#include <http/uri.hh>
#include <http/msgdata.hh>
#include <dataman/buffer.hh>


using std::string;
using dataman::buffer;


http::msgdata::~msgdata()
{
}


string&	http::msgdata::operator[](const string&)
{
  static string fake_;
  return fake_;
}


bool	http::msgdata::parse_rqstline(buffer&, const uri&)
{
  return false;
}


bool	http::msgdata::build_respline(msgdata& rqst, buffer&, const uri&)
{
  return false;
}


string&	http::msgdata::query_string()
{
  static string fake_;
  return fake_;
}
