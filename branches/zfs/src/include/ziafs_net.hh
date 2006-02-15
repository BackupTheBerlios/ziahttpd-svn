//
// ziafs_net.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 16:47:08 2006 texane
// Last update Wed Feb 15 04:05:03 2006 
//


#ifndef ZIAFS_NET_HH
# define ZIAFS_NET_HH


#include <sys/sysapi.hh>


// forward declarations
struct core;
namespace net { class config; }

namespace net
{
  class server
  {
    friend void* thr::pool::server_entry(thr::pool::slot_t*);

  public:
    server(struct core*);

    bool reload_config(config*);
    bool run();

  private:
    // Data related
    struct core* core;

    // Network related
    bool is_bound;
    sysapi::insock::handle_t srv_sock;
    struct sockaddr_in srv_addr;
    unsigned int nr_bklog;
  };
}


#endif // ! ZIAFS_NET_HH
