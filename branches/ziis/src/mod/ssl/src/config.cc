#include "include/modimpl.hh"

utils::config::config(const std::string &config_file)
{
  if (!m_xmldoc.LoadFile(config_file.c_str(), TIXML_DEFAULT_ENCODING))
    {
      load_default();
      return ;
    }
  parse();
}

utils::config::config(char **av)
{

}

utils::config::config()
{
//	load_default();
}

utils::config::~config()
{
	std::list<server*>::iterator it;

	for(it = m_lserver.begin(); it != m_lserver.end(); ++it)
		delete *it;
}

bool	utils::config::load_default()
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

bool	utils::config::reset()
{
	m_ldirectory.clear();
	m_lserver.clear();
	load_default();
	return (true);
}

bool	utils::config::parse()
{
	key	key_s[] = {"server", "directory", "mime", "module", "system", "certif", ""};
	pFunc funcArray[] = {&utils::config::parse_server,
			     &utils::config::parse_directory,
			     &utils::config::parse_mimes,
			     &utils::config::parse_modules,
			     &utils::config::parse_system,
			     &utils::config::parse_certif};
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

bool	utils::config::parse_system()
{
	TiXmlElement*	xmltmp;

	for(xmltmp = m_xmlnode->FirstChildElement();
		xmltmp;
		xmltmp = xmltmp->NextSiblingElement())
	{
		if (xmltmp->ValueStr() == "nb_thread")
			m_system.nb_thread =  atoi(xmltmp->GetText());
		if (xmltmp->ValueStr() == "poll_delay")
			m_system.poll_delay = atol(xmltmp->GetText());
		if (xmltmp->ValueStr() == "expiration_delay")
			m_system.expiration_delay = atol(xmltmp->GetText());
		if (xmltmp->ValueStr() == "directory_listing")
			m_system.directory_listing = xmltmp->GetText();

	}
	return (true);
}

bool	utils::config::parse_server()
{
	server	*p = new server;
	TiXmlElement*	xmltmp;

	for(xmltmp = m_xmlnode->FirstChildElement();
		xmltmp;
		xmltmp = xmltmp->NextSiblingElement())
	{
		if (xmltmp->ValueStr() == "id")
			p->id.push_back(atoi(xmltmp->GetText()));
		if (xmltmp->ValueStr() == "cgi_id")
			p->cgi_id.push_back(atoi(xmltmp->GetText()));
		if (xmltmp->ValueStr() == "port")
			p->port = atoi(xmltmp->GetText());
		if (xmltmp->ValueStr() == "nr_bklog")
			p->nr_bklog = atoi(xmltmp->GetText());
		if (xmltmp->ValueStr() == "modulename")
			p->modulename.push_back(xmltmp->GetText());
		if (xmltmp->ValueStr() == "directory_index")
			stringmanager::split(xmltmp->GetText(), " ", p->directory_index);
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


bool	utils::config::parse_certif()
{
  certif *c = new certif;
  TiXmlElement*	xmltmp;

  xmltmp = m_xmlnode->ToElement();
  for(xmltmp = m_xmlnode->FirstChildElement(); xmltmp; xmltmp = xmltmp->NextSiblingElement())
    {
      if (xmltmp->ValueStr() == "pem_privkey")
	c->pem_privkey = xmltmp->GetText();
      else if (xmltmp->ValueStr() == "pem_cacert")
	c->pem_certif = xmltmp->GetText();
    }
  m_lcertifs.push_front(c);
  return (true);
}


bool	utils::config::parse_modules()
{
  module* m = new module;
  TiXmlElement*	xmltmp;

  //	memset((void *)m, 0, sizeof(*m));
  xmltmp = m_xmlnode->ToElement();
  m->name = xmltmp->FirstAttribute()->Value();
  for (xmltmp = m_xmlnode->FirstChildElement(); xmltmp; xmltmp = xmltmp->NextSiblingElement())
    {
      if (xmltmp->ValueStr() == "file")
	m->file = xmltmp->GetText();
    }
  m_lmodules.push_front(m);	
  return (true);
}


bool	utils::config::parse_directory()
{
	directory		*d = new directory;
	TiXmlElement*	xmltmp;

//	memset((void *)d, 0, sizeof(*d));
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

bool	utils::config::parse_mimes()
{
	mime			*m = new mime;
	TiXmlElement*	xmltmp;

//	memset((void *)m, 0, sizeof(*m));
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
		if (xmltmp->ValueStr() == "is_cgi" && xmltmp->GetText())
			m->is_cgi = atoi(xmltmp->GetText());
	}
	m_lmimes.push_front(m);	
	return (true);
}


bool utils::config::get_certif(std::list<utils::config::certif*>::iterator& it)
{
  it = m_lcertifs.begin();
  return true;
}

bool utils::config::end_certif(const std::list<utils::config::certif*>::iterator& it)
{
  if (it == m_lcertifs.end())
    return true;
  return false;
}

bool	utils::config::get_server(std::list<server*>::iterator &it)
{
	it = m_lserver.begin();
	return (true);
}

bool	utils::config::end_server(const std::list<server*>::iterator &it)
{
	if (it != m_lserver.end())
		return (false);
	return (true);
}

bool	utils::config::get_directory(std::list<directory*>::iterator &it)
{
	it = m_ldirectory.begin();
	return (true);
}

bool	utils::config::end_directory(const std::list<directory*>::iterator &it)
{
	if (it != m_ldirectory.end())
		return (false);
	return (true);
}

bool	utils::config::get_mimes(std::list<mime*>::iterator &it)
{
	it = m_lmimes.begin();
	return (true);
}

bool	utils::config::end_mimes(const std::list<mime*>::iterator &it)
{
	if (it != m_lmimes.end())
		return (false);
	return (true);
}

bool	utils::config::get_modules(std::list<module*>::iterator &it)
{
	it = m_lmodules.begin();
	return (true);
}

bool	utils::config::end_modules(const std::list<module*>::iterator &it)
{
	if (it != m_lmodules.end())
		return (false);
	return (true);
}
