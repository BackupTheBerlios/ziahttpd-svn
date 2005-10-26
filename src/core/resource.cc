//
// resource.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:49:09 2005 
// Last update Wed Oct 26 16:02:51 2005 
//


#include <string>
#include <sstream>
#include <resource.hh>


using std::ostringstream;
using std::string;


http::dataman::resource::resource()
{
}


http::dataman::resource::resource(const uri& u) : uri_(u), buf_(u)
{
}


http::dataman::resource::~resource()
{
}


string  http::dataman::resource::type() const
{
  ostringstream strm;
  strm << "<UNKNOWN_TYPE>";
  return strm.str();
}


const char* http::dataman::resource::universal_name() const
{
  return "UNKNOWN UNIVERSAL NAME";
}


const char* http::dataman::resource::local_name() const
{
  return "UNKNOWN LOCAL NAME";
}


// Check for the resource accessibility, and fill in
// the resource error code
bool http::dataman::resource::access()
{
  return false;
}


// Fetch the resource from the specified uri(ie. GET, POST... requests)
bool http::dataman::resource::fetch(const uri&)
{
  return false;
}


// Dump the resource to the specified uri(ie. PUT... requests)
bool http::dataman::resource::dump(const uri& u)
{
  return false;
}
