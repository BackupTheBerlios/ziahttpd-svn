//
// uri.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:48:55 2005 
// Last update Tue Nov  8 13:07:27 2005 
//


#include <string>
#include <uri.hh>


using std::string;


http::dataman::uri::uri()
{
}


http::dataman::uri::uri(const string& universal_name)
{
  // turn it into a local path
}


http::dataman::uri::~uri()
{}


// Universal name accessor
string& http::dataman::uri::universal_name()
{
  return universal_name_;
}


// Local name accessor
string& http::dataman::uri::local_name()
{
  return local_name_;
}


// Static member function
string http::dataman::uri::map_universal_to_local(const std::string& universal_name)
{
  string fake;
  return fake;
}


// Static member function
string http::dataman::uri::map_local_to_universal(const std::string& local)
{
  string fake;
  return fake;
}
