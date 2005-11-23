//
// session.cc<2> for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:46:45 2005 
// Last update Wed Nov 23 21:35:11 2005 texane
//


#include <http/uri.hh>
#include <http/session.hh>
#include <http/msgdata.hh>
#include <dataman/conf.hh>
#include <server/core.hh>
#include <server/service.hh>


http::session::session(dataman::conf& conf) : conf_(conf)
{
  services_ = server::core::services_;
  persistent_ = true;
  resource_ = 0;
}


http::session::~session()
{
}
