//
// session.cc<2> for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:46:45 2005 
// Last update Mon Nov 14 16:20:29 2005 
//


#include <http/uri.hh>
#include <http/session.hh>
#include <http/msgdata.hh>
#include <dataman/conf.hh>
#include <sysapi/sysapi.hh>
#include <server/core.hh>
#include <server/service.hh>


http::session::session(dataman::conf& conf) : conf_(conf)
{
  services_ = server::core::services_;
}


http::session::~session()
{
}


sysapi::socket_in::handle_t& http::session::hsock_con()
{
  return hsock_con_;
}


sysapi::socket_in::handle_t& http::session::hsock_srv()
{
  return hsock_srv_;
}
