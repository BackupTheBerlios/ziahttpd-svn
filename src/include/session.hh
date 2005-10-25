//
// session.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 13:54:54 2005 texane
// Last update Tue Oct 25 22:38:48 2005 
//


#ifndef SERVER_SESSION_HH
# define SERVER_SESSION_HH


#include <channel.hh>
#include <sysapi.hh>
#include <dataman.hh>
#include <buffer.hh>

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

    // Response body fetching
    bool body_fetch();

    // Create a worker thread
    bool create_worker_thread();

    // Getting the request from the network
    bool skip_crlf(char**, sysapi::socket_in::error_t*);

    bool get_statusline(http::dataman::buffer&, sysapi::socket_in::error_t*);
    bool get_headerline(http::dataman::buffer&, sysapi::socket_in::error_t*);
    bool get_body(http::dataman::buffer&, sysapi::socket_in::error_t*);

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

      // Request body
      bool is_body_;
      bool is_chunked_;
      http::dataman::buffer request_body_;

//       sysapi::socket_in::size_t sz_body_;
//       unsigned char* buf_body_;


      // Response body
      http::dataman::buffer response_body_;


      
      // Cgi script related
      bool is_cgi_;
      char* buf_cgi_;
      sysapi::socket_in::size_t sz_cgi_;

      // Body is read from a file
      bool is_file_;
      char* filename_;

      // status line and headers related
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
