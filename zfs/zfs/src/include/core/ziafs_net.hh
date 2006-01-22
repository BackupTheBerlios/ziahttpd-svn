//
// ziafs_net.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:44:51 2006 texane
// Last update Sun Jan 22 13:52:03 2006 texane
//


#ifndef ZIAFS_NET_HH
# define ZIAFS_NET_HH


#include <string>
#include <core/ziafs_buffer.hh>
#include <core/ziafs_status.hh>


// Forward declarations
namespace io { class resource; }
namespace io { class manager; };
namespace net { class protocol; }
namespace net { class config; }



namespace net
{
  class server
  {
  public:
    server(config*);
    server(char**);
    server(std::string&);
    ~server();

  private:
    // internal data
    config* m_config;
    io::manager* m_ioman;

    // internal management routines
    void reset();
  };
}

namespace net
{
  class session
  {
  public:
    session(io::resource* client, config*);
    ~session();

    // Accessor

  private:
    io::resource* m_client;
    io::resource* m_target;
    protocol* m_proto;
    config* m_config;
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
