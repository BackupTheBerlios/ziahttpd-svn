//
// session.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 13:54:54 2005 texane
// Last update Thu Oct 20 17:24:44 2005 
//


#ifndef SERVER_SESSION_HH
# define SERVER_SESSION_HH


#include <channel.hh>
#include <sysapi.hh>


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

    // Get non chunked body
    bool get_body(sysapi::socket_in::size_t);

    // get chunked body
    bool get_body();

  private:
    typedef struct http_info
    {
      // timing information
      int nr_rqst_;
      int max_rqst_;

      // Persistent connection information
      bool is_persistent_;

      // client information
    } http_info_t;


    // configuration
    // server::config* config_;

    // execution thread
    sysapi::thread::handle_t hdl_worker_;

    // Pointer to channel's session
    server::channel* this_chan_;

    // Connection socket related
    sysapi::socket_in::handle_t hdl_con_;

    // Thread worker entry point
    static sysapi::thread::retcode_t worker_entry_(sysapi::thread::param_t);

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
