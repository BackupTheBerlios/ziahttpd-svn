//
// channel.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 10:46:47 2005 texane
// Last update Thu Oct 20 21:17:46 2005 
//


#ifndef SERVER_CHANNEL_HH
# define SERVER_CHANNEL_HH


#include <list>
#include <sysapi.hh>
#include <core.hh>


// A channel is a way to keep track of sessions for a
// given (local_addr, local_port).
// Every channel has its own dispatcher thread, that blocks
// on accept and "dispatch" the work to do on a worker thread.

namespace server
{
  class session;

  class channel
  {
  public:
    channel(unsigned short, const char* = NULL);
    channel(const char*, const char* = NULL);
    ~channel();

    sysapi::thread::handle_t dispatcher_handle() const { return hdl_dispatcher_; }

  private:
    sysapi::socket_in::handle_t	hdl_accept_;
    sysapi::mutex::handle_t	lock_accept_;
    struct sockaddr_in		addr_accept_;

    sysapi::thread::handle_t	hdl_dispatcher_;

    std::list<server::session*>	list_sess_;

    static sysapi::thread::retcode_t server::channel::dispatcher_entry(sysapi::thread::param_t);
    static sysapi::thread::retcode_t thread_entry_(sysapi::thread::param_t);
  };
}


#endif // ! SERVER_CHANNEL_HH
