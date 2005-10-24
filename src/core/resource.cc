//
// resource.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:49:09 2005 
// Last update Mon Oct 24 15:32:08 2005 
//


#include <resource.hh>


http::dataman::resource::resource(const uri& u) : uri_(u), buf_(u)
{
}


http::dataman::resource::~resource()
{
}


inline const char* http::dataman::resource::type() const
{
  return "UNKNOWN TYPE";
}


inline const char* http::dataman::resource::universal_name() const
{
  return "UNKNOWN UNIVERSAL NAME";
}


inline const char* http::dataman::resource::local_name() const
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
