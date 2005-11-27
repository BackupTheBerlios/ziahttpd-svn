//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/branches/ziahttpd-mod/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Nov 16 11:42:46 2005 
// Last update Sun Nov 27 03:40:12 2005 texane
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


// Services exported by the server,
// stored here for the moment.
server::service* services_;


// Exported function definitions

MOD_EXPORT( HK_CREATE_CONNECTION )(http::session& session, server::core*, int&)
{
  // Register a new session
  // for the newly created
  // connection.

  services_ = session.services_;
  cout << "in the creation connection hook" << endl;

  return true;
}


MOD_EXPORT( HK_GET_RQST_METADATA )(http::session& session, server::core*, int&)
{
  // ?
  // Read medata from the network.
  // For zia, meta data default reading function
  // reads http compliant lines from the network.

  // Register request data reading callback
  cout << "Reading metadata" << endl;
  session.services_->perform_io(session.hsock_con(),
				server::service::EVREAD,
				read_metadata);
  
  return false;
}


MOD_EXPORT( HK_GET_RQST_DATA )(http::session& session, server::core*, int&)
{
  // ?
  // Read data from the network.
  // For zia, data default reading function
  // reads http compliant lines from the network.

  // Register request data reading callback
  cout << "Reading data" << endl;
  if (session.content_in().size())
    {
      session.services_->perform_io(session.hsock_con(),
				    server::service::EVREAD,
				    read_data);
      return false;
    }
  
  return true;
}


MOD_EXPORT( HK_SEND_RESPONSE)(http::session& session, server::core*, int&)
{
  // Register a new callback to be
  // called at send time

  // Sending response
  cout << "Sending the response" << endl;
  if (session.content_out().size() ||
      session.hdrlines_out().size())
    {
      session.services_->perform_io(session.hsock_con(),
				    server::service::EVWRITE,
				    send_response);
      return false;
    }

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

  session.services_->perform_io(session.hsock_con(),
				server::service::EVCLOSE,
				close_connection);

  return false;
}
