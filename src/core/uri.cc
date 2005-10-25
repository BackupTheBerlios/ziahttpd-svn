//
// uri.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:48:55 2005 
// Last update Tue Oct 25 23:32:17 2005 
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
const string& http::dataman::uri::universal_name() const
{
  return universal_name_;
}


// Local name accessor
const string& http::dataman::uri::local_name() const
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
