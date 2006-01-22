//
// ziafs_net.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:44:51 2006 texane
// Last update Sun Jan 22 17:41:05 2006 texane
//


#ifndef ZIAFS_NET_HH
# define ZIAFS_NET_HH


#include <string>
#include <core/ziafs_buffer.hh>
#include <core/ziafs_status.hh>
#include <config/tinyxml.hh>
#include <list>

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
    server(const std::string&);
    ~server();

    status::error process_requests();

  private:
    // internal data
    config* m_config;
    io::manager* m_ioman;

    // internal state
    bool m_done;

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

    // protocol interface
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
  public:
	  struct protocol
	  {
		  int			id;
		  int			port;
		  std::string	type;
	  };
  public:
	  config(const std::string &);
	  config();
	  ~config();
	  config(char **);
	  bool		reset();
	  bool		get_protocol(std::list<protocol*>::iterator&);
	  bool		end_protocol(const std::list<protocol*>::iterator&);
  private:
	  bool			load_default();
	  bool			parse();
	  TiXmlDocument m_xmldoc;
	  TiXmlNode*	m_xmlnode;
	  bool			parse_protocol();
	  std::list<protocol*>	m_lprotocol;
  };
}

#endif // ! ZIAFS_NET_HH
