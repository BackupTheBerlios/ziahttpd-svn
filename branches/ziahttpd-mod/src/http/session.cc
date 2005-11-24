//
// session.cc<2> for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:46:45 2005 
// Last update Thu Nov 24 13:01:17 2005 texane
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
  nrpassed_ = 0;
}


http::session::~session()
{
}


bool	http::session::reset()
{
  // Wait for the msgdata clear method implementation
  // Keep the handle on the connection opened
  // Keep the conf
  // Keep the services

  // reset the resource, if any
  if (resource_)
    {
      delete resource_;
      resource_ = 0;
    }

  // In case...
  persistent_ = true;

  // Clear in information
  // ! info_in_.clear();
  content_in_.clear();
  hdrlines_in_.clear();

  // Clear out informations
  // ! info_out_.clear();
  hdrlines_out_.clear();
  content_out_.clear();

  // Clear the uri
  // ? Ask mat to alias the uri.reset -> uri.clear
  // uri_.clear();
  uri_.reset();

  // Increment the pass count
  ++nrpassed_;
  
  return true;
}
