#include <core/ziafs_net.hh>
#include <iostream>

net::config::config(std::string &config_file)
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

bool	net::config::load_default()
{
	//load default config
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
	m_lprotocol.push_back(p);

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
							