//
// ziafs_net.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:44:51 2006 texane
// Last update Thu Jan 26 01:21:00 2006 texane
//


#ifndef ZIAFS_NET_HH
# define ZIAFS_NET_HH


#include <string>
#include <core/ziafs_buffer.hh>
#include <core/ziafs_status.hh>
#include <core/ziafs_utils.hh>
#include <core/ziafs_mod.hh>
#include <config/tinyxml.hh>
#include <list>
#include <map>

// Forward declarations
namespace io { class resource; }
namespace io { class res_manager; }
namespace net { class protocol; }
namespace net { class config; }
namespace net { class session; }
namespace net { class server; }
namespace net { class mod_manager; }



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
    net::mod_manager* m_modman;

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

  // structure semantic
  class session
  {
  public:
    // methods
    session(io::resource* client, config*, protocol*);
    ~session();
    status::error process();

    // attributes
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
    virtual ~protocol() {}

    // protocol interface
    virtual status::error consume(session*, buffer&) = 0;
    virtual status::error produce(buffer&) = 0;
		virtual	status::error	dump(buffer&) = 0;
		// switch function pointer
    status::error (*process_stage_fn)(session*);
    
  };

	class uri
	{
	public:
		static status::error	extension(std::string& localname);
		std::string&					localname() { return m_localname; };
		std::string&					wwwname() { return m_wwwname; };
		int&									status_code() { return m_status_code; };
	private:
		std::string	m_wwwname;
		std::string	m_localname;
		int	m_status_code;
	};

  class http : public protocol
  {
  public:
		http();
    std::string& operator[](const std::string&);
    std::string& operator=(const std::string&);
    status::error					consume(session*, buffer&);
    status::error					produce(buffer&);
		static status::error	first_stage(session*);
		static status::error	second_stage(session*);
		status::error					dump(buffer&);
	private:
		status::error	parse_status_line(std::string&);
		status::error	parse_header_line(std::string&);
    std::map<std::string, std::string> m_hdrlines;
    utils::line m_line;
		enum e_state
		{
			STUSLINES,
			HDRLINES,
			BODYDATA,
		};
		e_state			m_state;
		std::string	m_method;
		std::string	m_version;
		std::string	m_query;
		uri					m_uri;
  };

}

namespace net
{
  class config
  {
  public:
		struct module
		{
			std::string	file;
			std::string	name;
		};
    struct protocol
    {
      int			id;
      int			port;
      std::string	modulename;
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
		status::error		dump(buffer &buf);
    bool						reset();
    bool						get_protocol(std::list<protocol*>::iterator&);
    bool						end_protocol(const std::list<protocol*>::iterator&);
    bool						get_directory(std::list<directory*>::iterator&);
    bool						end_directory(const std::list<directory*>::iterator&);
    bool						get_mimes(std::list<mime*>::iterator&);
    bool						end_mimes(const std::list<mime*>::iterator&);
		bool						get_modules(std::list<module*>::iterator&);
		bool						end_modules(const std::list<module*>::iterator&);

  private:
    typedef bool (net::config::*pFunc)();
    struct	key
    {
      std::string	keyword;
    };
    bool						load_default();
    bool						parse();
    bool						parse_protocol();
    bool						parse_directory();
    bool						parse_mimes();
    bool						parse_modules();
		TiXmlDocument		m_xmldoc;
		TiXmlNode*			m_xmlnode;

    std::list<protocol*>	m_lprotocol;
    std::list<directory*>	m_ldirectory;
    std::list<mime*>			m_lmimes;
		std::list<module*>		m_lmodules;
  };
}

#endif // ! ZIAFS_NET_HH
