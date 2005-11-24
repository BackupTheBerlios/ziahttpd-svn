//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/branches/ziahttpd-mod/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Nov 16 11:42:46 2005 
// Last update Thu Nov 24 14:52:07 2005 texane
//


// ?
// This module implement zia default behaviour regarding
// netword data handling (that's reading and writing).
// It should always be included in your server, unless you
// made a this-like module.


#include <list>
#include <string>
#include <cstring>
#include <iostream>
#include <zia.hh>
#include <callback.hh>


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::list;


// List of exported functions
MOD_EXPORT( HK_CREATE_CONNECTION )(http::session&, server::core*, int&);
MOD_EXPORT( HK_GET_RQST_METADATA )(http::session&, server::core*, int&);
MOD_EXPORT( HK_GET_RQST_DATA )(http::session&, server::core*, int&);
MOD_EXPORT( HK_SEND_RESPONSE)(http::session&, server::core*, int&);
MOD_EXPORT( HK_RELEASE_CONNECTION)(http::session&, server::core*, int&);


// Exported function definitions

MOD_EXPORT( HK_CREATE_CONNECTION )(http::session& session, server::core*, int&)
{
  // ?
  // Create connection.
  // Our basic iomanager is based on io multiplexing,
  // so this function needs to export a function to be called
  // at close time.

  return true;
}


MOD_EXPORT( HK_GET_RQST_METADATA )(http::session& session, server::core*, int&)
{
  // ?
  // Read medata from the network.
  // For zia, meta data default reading function
  // reads http compliant lines from the network.

  // Register request data reading callback
  if (session.services_->register_callback(session, server::service::EVREAD, read_httpheaders) == false)
    return false;

  cout << "METADATA ENTER" << endl;
  session.services_->perform_io(session, server::service::EVREAD);

  return true;
}


MOD_EXPORT( HK_GET_RQST_DATA )(http::session& session, server::core*, int&)
{
  // ?
  // Read data from the network.
  // For zia, data default reading function
  // reads http compliant lines from the network.

  // Register the callback, and performs the
  // read call.

  cout << "READ DATA" << endl;

  if (session.services_->register_callback(session, server::service::EVREAD, read_httpbody) == false)
    return false;

  session.services_->perform_io(session, server::service::EVREAD);

  return true;
}


MOD_EXPORT( HK_SEND_RESPONSE)(http::session& session, server::core*, int&)
{
  // Register a new callback to be
  // called at send time
  if (session.services_->register_callback(session, server::service::EVWRITE, write_httpresponse) == false)
    return false;

  session.services_->perform_io(session, server::service::EVWRITE);

  return true;
}


MOD_EXPORT( HK_RELEASE_CONNECTION)(http::session& session, server::core*, int&)
{
  // ?
  // Relasing connection.
  // Our basic iomanager is based on io multiplexing,
  // so this function needs to export a function to be called
  // at close time.

  // ?
  // For the close connection hook, does error code matter

  if (session.persistent() == true)
    return true;

  cout << "closing connection" << endl;

  // Register a new callback for connection closing
  if (session.services_->register_callback(session, server::service::EVCLOSE, close_httpconnection) == false)
    return false;

  session.services_->perform_io(session, server::service::EVCLOSE);

  return true;
}
