//
// session.cc<2> for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:46:45 2005 
// Last update Thu Dec 01 21:45:35 2005 texane
//


#include <iostream>
#include <http/uri.hh>
#include <http/session.hh>
#include <http/msgdata.hh>
#include <dataman/conf.hh>
#include <server/core.hh>
#include <server/service.hh>
#include <server/modman.hh>


using std::cout;
using std::endl;


http::session::session(dataman::conf& conf) : conf_(conf)
{
  services_ = server::core::services_;
  persistent_ = true;
  resource_ = 0;
  nrpassed_ = 0;
  stageid_ = server::modman::CREATE_CON;
  reset_me_= false;
  handleio_ = false;
  chunked_ = false;
  first_chunk_ = false;
  last_chunk_ = false;
}


http::session::~session()
{
  cout << "\t[? " << std::dec << (unsigned int)hsock_con_ << " ] Session is being destructed" << endl;

  // Close the connection
  sysapi::socket_in::terminate_connection(hsock_con_);

  // If any resource are in use, release
  if (resource_)
    delete resource_;
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
  reset_me_ = false;
  chunked_= false;
  first_chunk_ = false;
  last_chunk_ = false;

  // Set the processing stage
  stageid_ = server::modman::CREATE_CON;

  // Clear in information
  info_in_.reset();
  content_in_.clear();
  hdrlines_in_.clear();

  // Clear out informations
  info_out_.reset();
  hdrlines_out_.clear();
  content_out_.clear();

  // Clear the uri
  // ? Ask mat to alias the uri.reset -> uri.clear
  uri_.reset();

  // Increment the pass count
  ++nrpassed_;
  handleio_ = false;
  
  return true;
}
