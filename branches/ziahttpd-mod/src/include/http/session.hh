//
// session.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 14:18:57 2005 
// Last update Mon Nov 14 16:19:14 2005 
//


#ifndef HTTP_SESSION_HH
# define HTTP_SESSION_HH


//
// [*] sessions
// Session is the basic enitity containing
// informations on a session initiated by an http client.
// It's the one passed to modules, in order
// for them to modify session related informations.


#include <dataman/conf.hh>
#include <dataman/buffer.hh>
#include <http/msgdata.hh>
#include <sysapi/sysapi.hh>
#include <server/service.hh>
#include <server/module.hh>


namespace http
{
  class session
  {
    
    //
    // Contains the current http session related informations
    // Sessions are passed to the modules by the means of hook in the code
    // in order for them to modify the request processing flow.

  public:
    session(dataman::conf&);
    ~session();

    sysapi::socket_in::handle_t& hsock_con();
    sysapi::socket_in::handle_t& hsock_srv();

    // Put here for the moment, avoiding undef sym in symbols ...
    server::service* services_;


  private:
    // Server configuration related
    dataman::conf& conf_;


    // Internet socket having accepted the connection
    sysapi::socket_in::handle_t hsock_srv_;

    // Internet socket handle for the current session connection
    sysapi::socket_in::handle_t hsock_con_;
    

    // Client request (in) related
    http::msgdata info_in_;
    dataman::buffer hdrlines_in_;
    dataman::buffer content_in_;

    // Requested resource naming related
    http::uri uri_;

    // Server response (out) related
    http::msgdata info_out_;
    dataman::buffer hdrlines_out_;
    dataman::buffer content_out_;
  };
}


//! \class http::session
//! \brief Http related request data storage class
//!
//! As the request is processing, the core has to
//! maintain the request changing state and data
//! associated.
//! In order to do so, the http::session is used.
//! This is this chunk of data that is passed to
//! modules for them to interact with the request
//! processing flow.




#endif // ! HTTP_SESSION_HH
