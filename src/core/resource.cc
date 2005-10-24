//
// resource.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:49:09 2005 
// Last update Mon Oct 24 14:57:12 2005 
//


#include <resource.hh>


http::dataman::resource::resource(const uri& u) : uri_(u), buf_(u)
{
}


http::dataman::resource::~resource()
{
}


void http::dataman::resource::display() const
{
}
