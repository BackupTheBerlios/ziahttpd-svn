#include <ziafs_http.hh>

using namespace sysapi;

status::error	net::uri::extension(std::string& localname)
{
	size_t i = m_wwwname.rfind(".", m_wwwname.length() - 1);
	if (i != std::string::npos)
	{
		localname = m_wwwname.substr(i + 1, m_wwwname.length() - i - 1);
	}
	ziafs_return_status(status::NOTIMPL);
}

bool				net::uri::reset()
{
	m_wwwname.clear();
	m_localname.clear();
	m_status_code = 0;
	return true;
}

net::uri::uri()
{
	reset();
}

std::string&	net::uri::localname(net::config& conf, std::string& host)
{
	std::list<net::config::directory*>::iterator	dir;
	std::list<net::config::server*>::iterator			serv;
	std::vector<std::string>::iterator						dir_index;
	std::string																		doc_root;

	conf.get_directory(dir);
	while (!conf.end_directory(dir))
	{
		if ((*dir)->servername == "*")
			doc_root = (*dir)->docroot + "/";
		if ((*dir)->servername == host)
			doc_root = (*dir)->docroot + "/";
		dir ++;
	}

	if (m_wwwname[m_wwwname.size() - 1] == '/')
	{
		// check directory listing
		int	have_dir = 0;
		conf.get_server(serv);
		for (dir_index = (*serv)->directory_index.begin(); dir_index != (*serv)->directory_index.end(); dir_index++)
		{
			std::string	tmp;

			tmp = doc_root + m_wwwname + (*dir_index);
			if (file::is_readable(tmp))
			{
				m_wwwname += (*dir_index);
				have_dir = 1;
				break;
			}
		}
	}
	m_localname = doc_root + m_wwwname;
	return m_localname;
}