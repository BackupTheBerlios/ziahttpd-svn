//
// resource.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:49:09 2005 
// Last update Wed Oct 26 17:07:17 2005 
//


#include <string>
#include <sstream>
#include <resource.hh>


using std::ostringstream;
using std::string;


http::dataman::resource::resource()
{
  reset();
}


http::dataman::resource::resource(const uri& u) : uri_(u), buf_(u)
{
  reset();
}


http::dataman::resource::~resource()
{
  reset();
}


void	http::dataman::resource::reset()
{
  cgi_ = false;
  raw_ = false;
  mod_ = false;

  local_ = false;
  get_ = false;
  post_ = false;
  put_ = false;
  
  errcode_ = 0;
}


const string&	http::dataman::resource::universal_name() const
{
  return uri_.universal_name();
}


const string&	http::dataman::resource::local_name() const
{
  return uri_.local_name();
}


// Check for the resource accessibility, and fill in the resource error code
bool		http::dataman::resource::access()
{
  return false;
}


// Fetch the resource from the specified uri(ie. GET, POST... requests)
bool http::dataman::resource::fetch(const uri&)
{
  return false;
}


// Dump the resource to the specified uri(ie. PUT... requests)
bool http::dataman::resource::dump(const uri&)
{
  return false;
}


// - Resource type related

bool		http::dataman::resource::is_cgi() const
{
  return cgi_;
}


bool		http::dataman::resource::is_raw() const
{
  return mod_;
}


bool		http::dataman::resource::is_mod() const
{
  return raw_;
}


// - Resource access method related

bool		http::dataman::resource::is_get() const
{
  return get_;
}


bool		http::dataman::resource::is_post() const
{
  return post_;
}


bool		http::dataman::resource::is_put() const
{
  return put_;
}


bool		http::dataman::resource::is_local() const
{
  return local_;
}
