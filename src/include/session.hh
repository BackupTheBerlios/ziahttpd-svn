//
// session.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 13:54:54 2005 texane
// Last update Wed Oct 19 23:47:14 2005 
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
    sysapi::socket_in::handle_t get_connection_handle() const { return hdl_con_; }
    server::channel* get_channel() const { return this_chan_; }

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
    sysapi::thread::handle_t hdl_thread_;

    // Pointer to channel's session
    server::channel* this_chan_;

    // Connection socket related
    sysapi::socket_in::handle_t hdl_con_;
  };
}


#endif // ! SERVER_SESSION_HH
