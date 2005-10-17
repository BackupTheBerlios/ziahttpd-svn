//
// session.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 13:54:54 2005 texane
// Last update Mon Oct 17 18:03:44 2005 
//


#ifndef SERVER_SESSION_HH
# define SERVER_SESSION_HH


#include <channel.hh>
#include <sockin.hh>
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
    session();
    ~session();

    // Entry point of the session's thread
    static sysapi::thread::retcode_t entrypoint_(sysapi::thread::param_t);

  private:
    typedef struct http_info
    {
      // timing information, keep alive...
      int nr_rqst_;
      int max_rqst_;

      // client information
      server::sockin_t sin_client_;
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
