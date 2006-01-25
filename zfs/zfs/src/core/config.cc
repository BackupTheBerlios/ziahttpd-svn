#include <core/ziafs_net.hh>
#include <iostream>
#include <core/ziafs_debug.hh>
#include <sstream>

net::config::config(const std::string &config_file)
{
	load_default();
	if (!m_xmldoc.LoadFile(config_file.c_str(), TIXML_DEFAULT_ENCODING))
	{
		ziafs_debug_msg("File not found :%s\n", config_file.c_str());
		return ;
	}
	parse();
}

net::config::config(char **av)
{

}

net::config::config()
{
	load_default();
}

net::config::~config()
{
	std::list<protocol*>::iterator it;

	for(it = m_lprotocol.begin(); it != m_lprotocol.end(); ++it)
		delete *it;
}

bool	net::config::load_default()
{
	//load default config
	const char* def_conf =
		"<?xml version=\"1.0\"  standalone='no' >\n"
		"<protocol>"
		"<id>1</id>"
		"<type>http</type>"
		"<port>80</port>"
		"</protocol>"
		"<directory id=1>"
		"<servername></servername>"
		"<docroot>test/www/</docroot>"
		"</directory>";

	m_xmldoc.Parse(def_conf);
	parse();
	return (true);
}

bool	net::config::reset()
{
	m_ldirectory.clear();
	m_lprotocol.clear();
	load_default();
	return (true);
}

bool	net::config::parse()
{
	key	key_s[] = {"protocol", "directory", "mime", ""};
	pFunc funcArray[] = {&net::config::parse_protocol, &net::config::parse_directory, &net::config::parse_mimes};
	int	i;
	for(m_xmlnode = m_xmldoc.FirstChild();
		m_xmlnode;
		m_xmlnode = m_xmlnode->NextSibling() )
	{
		for (i = 0; !key_s[i].keyword.empty(); i++)
			if (m_xmlnode->ValueStr() == key_s[i].keyword)
				(this->*funcArray[i])();
	}
	return (true);
}

bool	net::config::parse_protocol()
{
	protocol	*p = new protocol;
	TiXmlElement*	xmltmp;

	for(xmltmp = m_xmlnode->FirstChildElement();
		xmltmp;
		xmltmp = xmltmp->NextSiblingElement())
	{
		if (xmltmp->ValueStr() == "id")
			p->id = atoi(xmltmp->GetText());
		if (xmltmp->ValueStr() == "port")
			p->port = atoi(xmltmp->GetText());
		if (xmltmp->ValueStr() == "type")
			p->type = xmltmp->GetText();
	}
	std::list<protocol *>::iterator i;

	for(i = m_lprotocol.begin(); i != m_lprotocol.end(); ++i)
		if ((*i)->port == p->port)
		{
			(*i)->type = p->type;
			(*i)->id = p->id;
			delete	p;
			return (true);
		}
	m_lprotocol.push_front(p);	
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

bool	net::config::get_protocol(std::list<protocol*>::iterator &it)
{
	it = m_lprotocol.begin();
	return (true);
}

bool	net::config::end_protocol(const std::list<protocol*>::iterator &it)
{
	if (it != m_lprotocol.end())
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

bool	net::config::dump(buffer &buf)
{
	std::list<protocol *>::iterator ip;
	std::list<directory *>::iterator id;
	std::list<mime *>::iterator im;

	buf += "<protocol>\n";
	for(ip = m_lprotocol.begin(); ip != m_lprotocol.end(); ++ip)
	{
		std::ostringstream stream;
		stream << "  ID :" << (*ip)->id << " TYPE :" << (*ip)->type << " PORT :" << (*ip)->port << "\n";
		buf += stream.str();
	}
	buf += "</protocol>\n";
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
	return (true);
}
