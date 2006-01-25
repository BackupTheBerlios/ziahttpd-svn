//
// ziafs_net.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:44:51 2006 texane
// Last update Wed Jan 25 21:33:40 2006 texane
//


#ifndef ZIAFS_NET_HH
# define ZIAFS_NET_HH


#include <string>
#include <core/ziafs_buffer.hh>
#include <core/ziafs_status.hh>
#include <core/ziafs_utils.hh>
#include <config/tinyxml.hh>
#include <list>
#include <map>

// Forward declarations
namespace io { class resource; }
namespace io { class res_manager; };
namespace net { class protocol; }
namespace net { class config; }
namespace net { class session; }
namespace net { class server; }



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
    io::res_manager* m_resman;

    // set of session
    std::list<session*> m_sessions;

    // internal state
    bool m_done;

    // internal management routines
    void reset();
    void init();
    void release();
  };
}

namespace net
{
  friend class server;

  class session
  {
  public:
    session(io::resource* client, config*);
    ~session();

    status::error process();

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
    virtual status::error consume(session*, buffer&) = 0;
    virtual status::error produce(buffer&) = 0;
  };


  class http : public protocol
  {
  public:
    http();
    std::string& operator[](const std::string&);
    std::string& operator=(const std::string&);
    status::error	consume(session*, buffer&);
    status::error	produce(buffer&);
  private:
    std::map<std::string, std::string> m_hdrlines;
    utils::line m_line;
    
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
    struct mime
    {
      std::string	extension;
      std::string	type;
      std::string	image;
      std::string	cgi;
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
    bool		get_mimes(std::list<mime*>::iterator&);
    bool		end_mimes(const std::list<mime*>::iterator&);

  private:
    typedef bool (net::config::*pFunc)();
    struct	key
    {
      std::string	keyword;
    };
    bool			load_default();
    bool			parse();
    TiXmlDocument m_xmldoc;
    TiXmlNode*	m_xmlnode;
    bool			parse_protocol();
    bool			parse_directory();
    bool			parse_mimes();

    std::list<protocol*>	m_lprotocol;
    std::list<directory*>	m_ldirectory;
    std::list<mime*>		m_lmimes;
  };
}

#endif // ! ZIAFS_NET_HH
