//
// session.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 14:18:57 2005 
// Last update Tue Nov 22 08:27:40 2005 texane
//


#ifndef HTTP_SESSION_HH
# define HTTP_SESSION_HH


//
// [*] sessions
// Session is the basic enitity containing
// informations on a session initiated by an http client.
// It's the one passed to modules, in order
// for them to modify session related informations.


#include <list>
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

    // Access the client dedicated connection handle.
    sysapi::socket_in::handle_t& hsock_con()    { return hsock_con_; }

    // Access the server dedicated connection handle,
    // bound on the local port.
    sysapi::socket_in::handle_t& hsock_srv()    { return hsock_srv_; }

    // {In, out}put (client's request, server's response) data related accessors
    std::list<dataman::buffer>& hdrlines_in()	{ return hdrlines_in_; }
    dataman::buffer& content_in()		{ return content_in_; }
    dataman::buffer& hdrlines_out()		{ return hdrlines_out_; }
    dataman::buffer& content_out()		{ return content_out_; }
    http::uri& uri()				{ return uri_; }
    http::msgdata& info_in()			{ return info_in_; }
    http::msgdata& info_out()			{ return info_out_; }

    // Get/Set the connection persistency
    bool& persistent()				{ return persistent_; }

    // Put here for the moment, avoiding undef sym in symbols ...
    server::service* services_;

  private:

    struct resource
    {
      // As connections are persistent
      // resources associated with it should
      // be too.
      
      // list of the current cgi scripts
      // Are there more than one cgi running
      // for a given session at a given time?
      struct cgi
      {
	int retcode_;
	sysapi::process::handle_t hproc_;
	// struct timeval tmelapsed_;
      };
      std::list<struct cgi> listproc_;
    };

    // Server configuration related
    dataman::conf& conf_;


    // Internet socket having accepted the connection
    sysapi::socket_in::handle_t hsock_srv_;

    // Internet socket handle for the current session connection
    sysapi::socket_in::handle_t hsock_con_;    

    // Client request (in) related
    http::msgdata info_in_;
    std::list<dataman::buffer> hdrlines_in_;
    dataman::buffer content_in_;

    // Requested resource naming related
    http::uri uri_;

    // Server response (out) related
    http::msgdata info_out_;
    dataman::buffer hdrlines_out_;
    dataman::buffer content_out_;

    // Informations related to http internals (?)
    // Connection persistency.
    bool persistent_;
    int nrpassed_;
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
