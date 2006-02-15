#include <ziafs_config.hh>
#include <iostream>
#include <ziafs_debug.hh>
#include <sstream>

net::config::config(const std::string &config_file)
{
	if (!m_xmldoc.LoadFile(config_file.c_str(), TIXML_DEFAULT_ENCODING))
	{
		ziafs_debug_msg("File not found :%s\n", config_file.c_str());
		load_default();
		return ;
	}
	parse();
	buffer t;
	dump(t);
//	std::cout << t.c_str();
	ziafs_debug_msg("%s", t.c_str());
}

net::config::config(char **av)
{

}

net::config::config()
{
//	load_default();
}

net::config::~config()
{
	std::list<server*>::iterator it;

	for(it = m_lserver.begin(); it != m_lserver.end(); ++it)
		delete *it;
}

bool	net::config::load_default()
{
	//load default config
	const char* def_conf =
		"<?xml version=\"1.0\"  standalone='no' >\n"
		"<protocol>"
		"  <id>1</id>"
		"  <module>./module/net/net.lo</module>"
		"  <port>80</port>"
		"</protocol>"
		"<directory id=1>"
		"  <servername></servername>"
		"  <docroot>test/www/</docroot>"
		"</directory>";

	m_xmldoc.Parse(def_conf);
	parse();
	return (true);
}

bool	net::config::reset()
{
	m_ldirectory.clear();
	m_lserver.clear();
	load_default();
	return (true);
}

bool	net::config::parse()
{
	key	key_s[] = {"server", "directory", "mime", "module", "system", ""};
	pFunc funcArray[] = {&net::config::parse_server,
											&net::config::parse_directory,
											&net::config::parse_mimes,
											&net::config::parse_modules,
											&net::config::parse_system};
	int	i;
	for(m_xmlnode = m_xmldoc.FirstChild();
		m_xmlnode;
		m_xmlnode = m_xmlnode->NextSibling())
	{
		for (i = 0; !key_s[i].keyword.empty(); i++)
			if (m_xmlnode->ValueStr() == key_s[i].keyword)
			{
				(this->*funcArray[i])();
			}
	}
	return (true);
}

bool	net::config::parse_system()
{
	TiXmlElement*	xmltmp;

	for(xmltmp = m_xmlnode->FirstChildElement();
		xmltmp;
		xmltmp = xmltmp->NextSiblingElement())
	{
		if (xmltmp->ValueStr() == "nb_thread")
			m_system.nb_thread =  atoi(xmltmp->GetText());
		if (xmltmp->ValueStr() == "time")
			m_system.time = atoi(xmltmp->GetText());
	}
	return (true);
}

bool	net::config::parse_server()
{
	server	*p = new server;
	TiXmlElement*	xmltmp;

	for(xmltmp = m_xmlnode->FirstChildElement();
		xmltmp;
		xmltmp = xmltmp->NextSiblingElement())
	{
		if (xmltmp->ValueStr() == "id")
			p->id.push_back(atoi(xmltmp->GetText()));
		if (xmltmp->ValueStr() == "port")
			p->port = atoi(xmltmp->GetText());
		if (xmltmp->ValueStr() == "modulename")
			p->modulename.push_back(xmltmp->GetText());
	}
	std::list<server *>::iterator i;

	for(i = m_lserver.begin(); i != m_lserver.end(); ++i)
		if ((*i)->port == p->port)
		{
			(*i)->modulename = p->modulename;
			(*i)->id = p->id;
			delete	p;
			return (true);
		}
	m_lserver.push_front(p);	
	return (true);
}

bool	net::config::parse_modules()
{
	module		*m = new module;
	TiXmlElement*	xmltmp;

	xmltmp = m_xmlnode->ToElement();
	m->name = xmltmp->FirstAttribute()->Value();
	for(xmltmp = m_xmlnode->FirstChildElement();
		xmltmp;
		xmltmp = xmltmp->NextSiblingElement())
	{
		if (xmltmp->ValueStr() == "file")
			m->file = xmltmp->GetText();
	}
	m_lmodules.push_front(m);	
	return (true);
}


bool	net::config::parse_directory()
{
	directory		*d = new directory;
	TiXmlElement*	xmltmp;

	d->servername = "";
	d->docroot = "./";
	xmltmp = m_xmlnode->ToElement();
	d->id = atoi(xmltmp->FirstAttribute()->Value());

	for(xmltmp = m_xmlnode->FirstChildElement();
		xmltmp;
		xmltmp = xmltmp->NextSiblingElement())
	{
		if (xmltmp->ValueStr() == "servername" && xmltmp->GetText())
			d->servername = xmltmp->GetText();
		if (xmltmp->ValueStr() == "docroot")
		{
			d->docroot = xmltmp->GetText();
			if (d->docroot[d->docroot.length() - 1] != '/')
				d->docroot += "/";
		}
	}

	std::list<directory*>::iterator i;
	
	for(i = m_ldirectory.begin(); i != m_ldirectory.end(); ++i)
		if ((*i)->id == d->id)
		{
			(*i)->docroot = d->docroot;
			(*i)->servername = d->servername;
			delete	d;
			return (true);
		}
	m_ldirectory.push_front(d);	
	return (true);
}

bool	net::config::parse_mimes()
{
	mime			*m = new mime;
	TiXmlElement*	xmltmp;

	xmltmp = m_xmlnode->ToElement();
	m->extension = xmltmp->FirstAttribute()->Value();

	for(xmltmp = m_xmlnode->FirstChildElement();
		xmltmp;
		xmltmp = xmltmp->NextSiblingElement())
	{
		if (xmltmp->ValueStr() == "type" && xmltmp->GetText())
			m->type = xmltmp->GetText();
		if (xmltmp->ValueStr() == "image" && xmltmp->GetText())
			m->image = xmltmp->GetText();
		if (xmltmp->ValueStr() == "cgi" && xmltmp->GetText())
			m->cgi = xmltmp->GetText();
	}
	m_lmimes.push_front(m);	
	return (true);
}

bool	net::config::get_server(std::list<server*>::iterator &it)
{
	it = m_lserver.begin();
	return (true);
}

bool	net::config::end_server(const std::list<server*>::iterator &it)
{
	if (it != m_lserver.end())
		return (false);
	return (true);
}

bool	net::config::get_directory(std::list<directory*>::iterator &it)
{
	it = m_ldirectory.begin();
	return (true);
}

bool	net::config::end_directory(const std::list<directory*>::iterator &it)
{
	if (it != m_ldirectory.end())
		return (false);
	return (true);
}

bool	net::config::get_mimes(std::list<mime*>::iterator &it)
{
	it = m_lmimes.begin();
	return (true);
}

bool	net::config::end_mimes(const std::list<mime*>::iterator &it)
{
	if (it != m_lmimes.end())
		return (false);
	return (true);
}

bool	net::config::get_modules(std::list<module*>::iterator &it)
{
	it = m_lmodules.begin();
	return (true);
}

bool	net::config::end_modules(const std::list<module*>::iterator &it)
{
	if (it != m_lmodules.end())
		return (false);
	return (true);
}

status::error	net::config::dump(buffer &buf)
{
	std::list<server *>::iterator ip;
	std::list<directory *>::iterator id;
	std::list<mime *>::iterator im;
	std::list<module *>::iterator imo;
	std::list<std::string>::iterator imoname;
	std::list<int>::iterator iserid;
	std::ostringstream stream;

	buf += "<system>\n";
	stream << "  NB THREAD :" << m_system.nb_thread << "\n";
	stream << "  TIME :" << m_system.time << "\n";
	buf += stream.str();
	buf += "</system>\n";
	
	buf += "<server>\n";
	for(ip = m_lserver.begin(); ip != m_lserver.end(); ++ip)
	{
		std::ostringstream stream;
		for(iserid = (*ip)->id.begin(); iserid != (*ip)->id.end(); ++iserid)
			stream << "  ID :" << (*iserid) << "\n";
		for(imoname = (*ip)->modulename.begin(); imoname != (*ip)->modulename.end(); ++imoname)
			stream << " MODULE :" << (*imoname) << "\n";
		stream << " PORT :" << (*ip)->port << "\n";
		buf += stream.str();
	}
	buf += "</server>\n";
	buf += "<modules>\n";
	for(imo = m_lmodules.begin(); imo != m_lmodules.end(); ++imo)
	{
		std::ostringstream stream;
		stream << "  NAME :" << (*imo)->name << " FILE :" << (*imo)->file << "\n";
		buf += stream.str();
	}
	buf += "</modules>\n";
	buf += "<directory>\n";
	for(id = m_ldirectory.begin(); id != m_ldirectory.end(); ++id)
	{
		std::ostringstream stream;
		stream << "  ID :" << (*id)->id << " SERVERNAME :" << (*id)->servername << " DOCROOT :" << (*id)->docroot << "\n";
		buf += stream.str();
	}
	buf += "</directory>\n";
	buf += "<mimes>\n";
	for(im = m_lmimes.begin(); im != m_lmimes.end(); ++im)
	{
		std::ostringstream stream;
		stream << "  EXT :" << (*im)->extension << " TYPE :" << (*im)->type << " IMG :" << (*im)->image << " CGI :" <<(*im)->cgi << "\n";
		buf += stream.str();
	}
	buf += "</mimes>\n";
	ziafs_return_status(status::SUCCESS);
}

bool						net::config::generate_config(config::server* s, config& conf)
{
	std::list<int>::iterator iserid;
	std::list<directory*>::iterator idirectory;
	std::list<mime*>::iterator imime;
	std::list<module*>::iterator imodules;

	m_lserver.push_back(s);
	memcpy((void*)&m_system, (const void*)conf.get_system(), sizeof(system));
	for(iserid = s->id.begin(); iserid != s->id.end(); ++iserid)
	{
		conf.get_directory(idirectory);
		for (; !conf.end_directory(idirectory); idirectory++)
		{
			if ((*idirectory)->id == (*iserid))
				m_ldirectory.push_back((*idirectory));
		}
	}
	conf.get_mimes(imime);
	for (; !conf.end_mimes(imime); imime++)
		m_lmimes.push_back((*imime));
	conf.get_modules(imodules);
	for (; !conf.end_modules(imodules); imodules++)
		m_lmodules.push_back((*imodules));
	return (true);
}