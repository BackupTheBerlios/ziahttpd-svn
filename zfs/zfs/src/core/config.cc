#include <core/ziafs_net.hh>
#include <iostream>

net::config::config(const std::string &config_file)
{
	load_default();
	if (!m_xmldoc.LoadFile(config_file.c_str(), TIXML_DEFAULT_ENCODING))
	{
		std::cout << "ERR" << std::endl;
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
		"</protocol>";

	m_xmldoc.Parse(def_conf);
	return (true);
}

bool	net::config::reset()
{
	load_default();
	return (true);
}

bool	net::config::parse()
{
	for(m_xmlnode = m_xmldoc.FirstChild();
		m_xmlnode;
		m_xmlnode = m_xmlnode->NextSibling() )
	{
		if (m_xmlnode->ValueStr() == "protocol")
			parse_protocol();
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
	m_lprotocol.push_front(p);

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
							