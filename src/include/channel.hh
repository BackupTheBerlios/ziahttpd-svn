//
// channel.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 10:46:47 2005 texane
// Last update Mon Oct 17 13:29:13 2005 texane
//


#ifndef SERVER_CHANNEL_HH
# define SERVER_CHANNEL_HH


#include <list>
#include <sysapi.hh>
#include <core.hh>


// A channel is a way to keep track of sessions for a
// given (local_addr, local_port). Whereas it can be many
// session by channels, there is only one thread by channel(one session)
// being blocked in an accept call.
// @see session.hh for more information on sessions

namespace server
{
  class session;

  class channel
  {
  public:
    // Construction
    channel(unsigned short, const char*);
    channel(const char*, const char*);

    // Destruction
    ~channel();

  private:
    // Handle on the accepting socket
    sysapi::socket_in::handle_t hdl_accept_;
    // Mutex on the accepting socket, only one session calls accept
    sysapi::mutex::handle_t lock_accept_;
    // Internet address of the accepting socket
    struct sockaddr_in addr_accept_;

    // List of sessions
    std::list<server::session*> list_sess_;

    // Entry point of the thread
    static sysapi::thread::retcode_t thread_entry_(sysapi::thread::param_t);
  };
}


#endif // ! SERVER_CHANNEL_HH
