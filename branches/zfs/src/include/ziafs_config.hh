#ifndef ZIAFS_CONFIG_HH
# define ZIAFS_CONFIG_HH

#include <list>
#include <ziafs_status.hh>
#include <config/tinyxml.hh>


namespace net
{
	class config;
	class config
	{
	public:
		struct system
		{
			int					nb_thread;
			int					poll_delay;
			int					expiration_delay;
		};
		struct module
		{
			std::string	file;
			std::string	name;
		};
		struct server
		{
			std::list<int>		id;
			int								port;
			unsigned int			nr_bklog;
			std::list<std::string>	modulename;
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
		bool						generate_config(server*, config&);
		bool						get_server(std::list<server*>::iterator&);
		bool						end_server(const std::list<server*>::iterator&);
		bool						get_directory(std::list<directory*>::iterator&);
		bool						end_directory(const std::list<directory*>::iterator&);
		bool						get_mimes(std::list<mime*>::iterator&);
		bool						end_mimes(const std::list<mime*>::iterator&);
		bool						get_modules(std::list<module*>::iterator&);
		bool						end_modules(const std::list<module*>::iterator&);
		system*					get_system() { return &m_system; };
	private:
		typedef bool (net::config::*pFunc)();
		struct	key
		{
			std::string	keyword;
		};
		bool						load_default();
		bool						parse();
		bool						parse_server();
		bool						parse_directory();
		bool						parse_mimes();
		bool						parse_modules();
		bool						parse_system();
		TiXmlDocument		m_xmldoc;
		TiXmlNode*			m_xmlnode;

		std::list<server*>	m_lserver;
		std::list<directory*>	m_ldirectory;
		std::list<mime*>			m_lmimes;
		std::list<module*>		m_lmodules;
		system								m_system;
	};
}

#endif