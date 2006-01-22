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
// + Math
#include <config/tinyxml.hh>
#include <list>
// - Math

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

    // protocol interface
    virtual status::error consume(buffer&) = 0;
    virtual status::error produce(buffer&) = 0;
  };


  class http : public protocol
  {
  };
}

// + Math

// HOW TO USE

//net::config	conf((std::string)"../test/root/config/test.xml");
//std::list<net::config::protocol*>::iterator t;
//conf.get_protocol(t);
//while (!conf.end_protocol(t))
//{
//	std::cout << (*t)->port << std::endl;
//	t++;
//}




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
// - Math

#endif // ! ZIAFS_NET_HH
