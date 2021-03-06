//
// session.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 13:54:54 2005 texane
// Last update Wed Oct 26 17:53:45 2005 
//


#ifndef SERVER_SESSION_HH
# define SERVER_SESSION_HH


#include <channel.hh>
#include <sysapi.hh>
#include <dataman.hh>
#include <buffer.hh>
#include <resource.hh>
#include <uri.hh>


// The session is the basic runnable execution
// unit of our server.
// It handles http client requests and responses.
// It emulates a server environement, with a proper
// root directory, configuration block...
// So it's a kind of virtual server for a given
// channel (channel = local address and, local port).
// @see channel.hh for information on channels.

namespace http
{
  class message;
}


namespace server
{
  class channel;

  class session
  {
    friend class http::message;


  public:
    // Construction, destruction
    session(sysapi::socket_in::handle_t, server::channel*);
    ~session();

    // Accessors
    server::channel* get_channel() const { return this_chan_; }
    sysapi::socket_in::handle_t get_connection_handle() const { return hdl_con_; }
    sysapi::thread::handle_t get_worker_handle() const { return hdl_worker_; }
    bool is_persistent() const { return http_info_.is_persistent_; }

    // Reset http related informations
    void reset_http_information();

    // Fetch the response resource
    bool body_fetch();
    bool fetch_response_resource();

    // Create a worker thread
    bool create_worker_thread();

    // Getting the request from the network
    bool skip_crlf(char**, sysapi::socket_in::error_t*);

    // get helper functions
    bool get_statusline(http::dataman::buffer&, sysapi::socket_in::error_t* = 0);
    bool get_headerline(http::dataman::buffer&, sysapi::socket_in::error_t* = 0);
    bool get_body(http::dataman::buffer&, sysapi::socket_in::error_t* = 0);

    // send helper functions
    bool send_statusline();
    bool send_headerlines();
    bool send_body();


  private:
    // Fetch the body, helper methods
    bool body_fetch_from_file();
    bool body_fetch_from_cgibin();


    // store http relative information, used
    // to pass information from parser to core.
    typedef struct
    {
      // method related
      bool is_method_get_;
      bool is_method_post_;

      // Connection persistency related
      int nr_rqst_;
      int max_rqst_;
      bool is_persistent_;

      // Request related
      http::dataman::resource	request_res_;
      http::dataman::buffer	request_body_;
      http::dataman::uri	request_uri_;
      bool			is_chunked_;

      // Response related
      http::dataman::buffer	response_body_;
      http::dataman::uri	response_uri_;
      http::dataman::resource	response_res_;
      
      // Which type of file
      bool is_cgi_;
      bool is_file_;

      // buffers for cstrings
      char* filename_;
      char* buf_statusline_;
      char* buf_headerlines_;

    } http_info_t;
    http_info_t http_info_;

    // store body relative information
    typedef struct
    {
      int nr_chunk_;
      unsigned char* buf_;
      sysapi::socket_in::size_t sz_;
    } body_chunk_t;
    body_chunk_t body_chunk_;


    // configuration
    // server::config* config_;

    server::channel* this_chan_;
    sysapi::socket_in::handle_t hdl_con_;

    static sysapi::thread::retcode_t worker_entry_(sysapi::thread::param_t);
    sysapi::thread::handle_t hdl_worker_;

    // timeout for the session, if in the idle state, in millisecond
    enum
      {
	MAX_IDLE_TIME = 3000,
      };
    long nr_timeout_;
  };
}


#endif // ! SERVER_SESSION_HH
