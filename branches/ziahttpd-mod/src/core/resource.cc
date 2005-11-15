//
// resource.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:49:09 2005 
// Last update Wed Oct 26 17:26:28 2005 
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


http::dataman::resource::resource(const buffer& buf) : buf_(buf)
{
  reset();
}


http::dataman::resource::~resource()
{
  reset();
}


// - Reset internal state of the resource

void		http::dataman::resource::reset()
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



// - Naming related

string&		http::dataman::resource::universal_name()
{
  return uri_.universal_name();
}


string&		http::dataman::resource::local_name()
{
  return uri_.local_name();
}


bool		http::dataman::resource::access()
{
  return false;
}


// - Resource buffer related

// Fetch the resource from the given buffer
bool		http::dataman::resource::fetch(const buffer& buf)
{
  return false;
}

// Fetch the resource from the given uri
bool		http::dataman::resource::fetch(const uri& uri)
{
  return false;
}

// Dump the resource content to the specified uri
bool		http::dataman::resource::dump(const uri& uri)
{
  return false;
}


// - Resource type related

bool&		http::dataman::resource::cgi()
{
  return cgi_;
}


bool&		http::dataman::resource::raw()
{
  return raw_;
}


bool&		http::dataman::resource::mod()
{
  return mod_;
}


// - Resource access method related

bool&		http::dataman::resource::get()
{
  return get_;
}


bool&		http::dataman::resource::post()
{
  return post_;
}


bool&		http::dataman::resource::put()
{
  return put_;
}


bool&		http::dataman::resource::local()
{
  return local_;
}
