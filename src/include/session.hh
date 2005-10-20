//
// session.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 13:54:54 2005 texane
// Last update Thu Oct 20 20:13:14 2005 
//


#ifndef SERVER_SESSION_HH
# define SERVER_SESSION_HH


#include <channel.hh>
#include <sysapi.hh>
#include <dataman.hh>


// The session is the basic runnable execution
// unit of our server.
// It handles http client requests and responses.
// It emulates a server environement, with a proper
// root directory, configuration block...
// So it's a kind of virtual server for a given
// channel (channel = local address and, local port).
// @see channel.hh for information on channels.


namespace server
{
  class channel;

  class session
  {
  public:
    // Construction, destruction
    session(sysapi::socket_in::handle_t, server::channel*);
    ~session();

    // Accessors
    server::channel* get_channel() const { return this_chan_; }
    sysapi::socket_in::handle_t get_connection_handle() const { return hdl_con_; }
    sysapi::thread::handle_t get_worker_handle() const { return hdl_worker_; }
    bool is_persistent() const { return is_persistent_; }

    // Create a worker thread
    bool create_worker_thread();

    // Getting the request from the network
    bool skip_crlf(char**, sysapi::socket_in::error_t*);
    bool get_headerlines(char**, sysapi::socket_in::error_t*);
    bool get_body(unsigned char**, sysapi::socket_in::size_t, sysapi::socket_in::size_t*, sysapi::socket_in::error_t*);
    bool get_body(unsigned char**, sysapi::socket_in::size_t*, sysapi::socket_in::size_t*, sysapi::socket_in::error_t*);

  private:

    // store http relative information
    typedef struct
    {
      int nr_rqst_;
      int max_rqst_;
      bool is_persistent_;
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
    bool is_persistent_;
  };
}


#endif // ! SERVER_SESSION_HH
