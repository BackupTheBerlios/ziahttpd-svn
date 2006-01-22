//
// ziafs_net.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:44:51 2006 texane
// Last update Sun Jan 22 13:24:53 2006 texane
//


#ifndef ZIAFS_NET_HH
# define ZIAFS_NET_HH


#include <ziafs_buffer.hh>
#include <ziafs_status.hh>


// Forward declarations
namespace io { class resource; }
namespace net { class protocol; }
namespace net { class config; }


namespace net
{
  class session
  {
  public:
    session(io::resource* client);
    ~session();

  private:
    io::resource* m_client;
    io::resource* m_target;
    protocol* m_proto;
    config* m_conf;
  };
}


namespace net
{
  class protocol
  {
  public:
    virtual ~protocol();

    // protcol interface
    virtual status::error consume(buffer&) = 0;
    virtual status::error produce(buffer&) = 0;
  };


  class http : public protocol
  {
  };
}


namespace net
{
  class config
  {
  };
}


#endif // ! ZIAFS_NET_HH
