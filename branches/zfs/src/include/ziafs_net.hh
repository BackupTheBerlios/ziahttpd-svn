//
// ziafs_net.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 16:47:08 2006 texane
// Last update Tue Feb 14 17:28:54 2006 texane
//


#ifndef ZIAFS_NET_HH
# define ZIAFS_NET_HH


// forward declarations
struct core;

namespace net
{
  class server
  {
  public:
    server(struct core*);

    bool reload_config();
    bool run();

  private:
    core* core;
  };
}


#endif // ! ZIAFS_NET_HH
