//
// session.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 13:54:54 2005 texane
// Last update Thu Oct 20 02:40:52 2005 
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

    // Create a worker thread
    bool create_worker_thread();

  private:
    typedef struct http_info
    {
      // timing information, keep alive...
      int nr_rqst_;
      int max_rqst_;

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
  };
}


#endif // ! SERVER_SESSION_HH
