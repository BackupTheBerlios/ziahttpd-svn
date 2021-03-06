//
// session.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 14:18:57 2005 
// Last update Sun Dec 04 17:11:16 2005 texane
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
#include <server/modman.hh>
#include <dataman/conf.hh>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>
#include <http/msgdata.hh>
#include <sysapi/sysapi.hh>


namespace server { class service; }


namespace http
{
  class session_manager;
  class session
  {
    
    //
    // Contains the current http session related informations
    // Sessions are passed to the modules by the means of hook in the code
    // in order for them to modify the request processing flow.

  public:

    friend class session_manager;

    session(dataman::conf&);
    ~session();

    // Reset the session
    bool reset();

    // Access the client dedicated connection handle.
    sysapi::socket_in::handle_t& hsock_con()    { return hsock_con_; }

    // Access the server dedicated connection handle,
    // bound on the local port.
    sysapi::socket_in::handle_t& hsock_srv()    { return hsock_srv_; }

    // Resource wanted to be accessed by the client
    // READ ONLY, call the service to CREATE
    dataman::resource* resource()		{ return resource_; }
    dataman::resource* resource_in()		{ return resource_in_; }

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
    
    // Is the session resource content chunked?
    bool& chunked()				{ return chunked_; }
    bool& first_chunk()				{ return first_chunk_; }
    bool& last_chunk()				{ return last_chunk_; }

    bool& handleio()				{ return handleio_; }

    // Configuration
    dataman::conf& conf()			{ return conf_; }

    // Put here for the moment, avoiding undef sym in symbols ...
    server::service* services_;


  private:
    friend class server::service;
    friend class server::core;
    friend class server::modman;

    // Processing stage
    server::modman::stageid_t stageid_;
    
    // Has the session to be reset?
    bool reset_me_;

    // resource pointed to by the client
    dataman::resource* resource_;
    dataman::resource* resource_in_;

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
    bool chunked_;
    bool first_chunk_;
    bool last_chunk_;

    // A call has been made to the io service
    bool handleio_;
  };
}


namespace http
{
  class session_manager
  {
    
  public:
    static http::session* factory(sysapi::socket_in::handle_t);
    static bool add(http::session*);
    static bool add(sysapi::socket_in::handle_t);
    static bool remove(session*);
    static bool remove(sysapi::socket_in::handle_t);
    static bool process();
    static bool find_byhdl(sysapi::socket_in::handle_t,
			   session*&);
    static bool to_string(session*);
    static bool to_string(sysapi::socket_in::handle_t);

  private:
    static std::list<http::session*> sessions_;

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
