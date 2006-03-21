//
// ziafs_net.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 16:47:08 2006 texane
// Last update Tue Mar 21 23:23:00 2006 texane
//


#ifndef ZIAFS_NET_HH
# define ZIAFS_NET_HH


#include <sys/sysapi.hh>
#include <ziafs_mod.hh>


// forward declarations
struct core;
namespace net { class config; }
namespace thr { class pool; }
class ZfsInput;

namespace net
{
  class server
  {
    friend class thr::pool;
    friend class ZfsInput;

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
    net::config* srv_config;
    unsigned int nr_bklog;

    // Http related

    // Module related
    mod::manager m_modman;
  };
}


#endif // ! ZIAFS_NET_HH
