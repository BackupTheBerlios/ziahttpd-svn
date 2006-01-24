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
#include <map>

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
  public:
	  http();
	  std::string& operator[](const std::string&);
	  std::string& operator=(const std::string&);
	  status::error	consume(buffer&);
	  status::error	produce(buffer&);
  private:
	  std::map<std::string, std::string> m_hdrlines;
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
	  struct directory
	  {
		  int			id;
		  std::string	servername;
		  std::string	docroot;
	  };
  public:
	  config(const std::string &);
	  config();
	  ~config();
	  config(char **);
	  bool		dump(buffer &buf);
	  bool		reset();
	  bool		get_protocol(std::list<protocol*>::iterator&);
	  bool		end_protocol(const std::list<protocol*>::iterator&);
	  bool		get_directory(std::list<directory*>::iterator&);
	  bool		end_directory(const std::list<directory*>::iterator&);

  private:
	  typedef bool (net::config::*pFunc)();
	  struct	key
	  {
		  std::string	keyword;
	  };

	  key			*key_s;
	  bool			load_default();
	  bool			parse();
	  TiXmlDocument m_xmldoc;
	  TiXmlNode*	m_xmlnode;
	  bool			parse_protocol();
	  bool			parse_directory();
	  bool			init_ptr();

	  std::list<protocol*>	m_lprotocol;
	  std::list<directory*>	m_ldirectory;
  };
}

#endif // ! ZIAFS_NET_HH
