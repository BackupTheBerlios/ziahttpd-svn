//
// resource_cgi.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:53:20 2005 texane
// Last update Wed Nov 23 16:09:38 2005 texane
//


#include <vector>
#include <string>
#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>


using std::vector;
using std::string;
using dataman::buffer;


dataman::cgi::cgi(const vector<const string>&,
		  const vector<const string>&,
		  const buffer&)
{
  
}


bool	dataman::cgi::open(error_t&)
{
  return false;
}


bool	dataman::cgi::fetch(buffer&, unsigned int, error_t&)
{
  return false;
}


bool	dataman::cgi::fetch(buffer&, error_t&)
{
  return false;
}


bool	dataman::cgi::close(error_t&)
{
  return false;
}
