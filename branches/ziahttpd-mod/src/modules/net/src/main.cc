//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/branches/ziahttpd-mod/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Nov 16 11:42:46 2005 
// Last update Wed Nov 23 11:05:43 2005 texane
//


// ?
// This module implement zia default behaviour regarding
// netword data handling (that's reading and writing).
// It should always be included in your server, unless you
// made a this-like module.


#include <zia.hh>
#include <list>
#include <string>
#include <cstring>
#include <iostream>


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


// Io callbacks, exported from callback.o
extern bool read_httpheaders(http::session*, server::service::iovec_t&);
extern bool read_httpbody(http::session*, server::service::iovec_t&);
extern bool read_cgistdout(http::session*, server::service::iovec_t&);
extern bool write_httpresponse(http::session*, server::service::iovec_t&);
extern bool write_cgistdin(http::session*, server::service::iovec_t&);
extern bool close_httpconnection(http::session*, server::service::iovec_t&);


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
  
  return session.services_->perform_io(session, server::service::EVREAD);
}


MOD_EXPORT( HK_GET_RQST_DATA )(http::session& session, server::core*, int&)
{
  // ?
  // Read data from the network.
  // For zia, data default reading function
  // reads http compliant lines from the network.

  // Register the callback, and performs the
  // read call.

  if (session.services_->register_callback(session, server::service::EVREAD, read_httpbody) == false)
    return false;

  return session.services_->perform_io(session, server::service::EVREAD);
}


MOD_EXPORT( HK_SEND_RESPONSE)(http::session& session, server::core*, int&)
{
  // Register a new callback to be
  // called at send time
  if (session.services_->register_callback(session, server::service::EVWRITE, write_httpresponse) == false)
    return false;

  // Cooking lesson:
  // Build the response buffer
  // wrap it into an iovec
  // and call the write service
  {
    server::service::iovec_t iov;
    iov.buf_ = session.hdrlines_out();
    iov.buf_ += session.content_out();
    session.services_->perform_io(session, server::service::EVWRITE);
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
  server::service::iovec_t iov;

  // Register a new callback for connection closing
  if (session.services_->register_callback(session, server::service::EVCLOSE, close_httpconnection) == false)
    return false;

  return session.services_->perform_io(session, server::service::EVCLOSE);
}
