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

bool				net::uri::is_cgi_extension(net::config& conf, std::string &ext)
{
	std::list<net::config::mime*>::iterator	imimes;

	conf.get_mimes(imimes);
	for (; !conf.end_mimes(imimes); imimes ++)
	{
		if (((*imimes)->extension == ext) && ((*imimes)->is_cgi == 1))
			return true;
	}
	return false;
}

std::string&	net::uri::localname(net::config& conf, std::string& host)
{
	std::list<net::config::directory*>::iterator	dir;
	std::list<net::config::server*>::iterator			serv;
	std::vector<std::string>::iterator						dir_index;
	std::string																		doc_root;
	std::string																		ext;

	conf.get_directory(dir);
	extension(ext);


	if (!is_cgi_extension(conf, ext))
	{
		while (!conf.end_directory(dir))
		{
			if (((*dir)->servername == "*")  && ((*dir)->id == 1))
				doc_root = (*dir)->docroot + "/";
			if (((*dir)->servername == host) && ((*dir)->id == 1))
				doc_root = (*dir)->docroot + "/";
			dir ++;
		}
	}
	else
	{
		while (!conf.end_directory(dir))
		{
			if (((*dir)->servername == "*") && ((*dir)->id == 2))
				doc_root = (*dir)->docroot + "/";
			if (((*dir)->servername == host) && ((*dir)->id == 2))
				doc_root = (*dir)->docroot + "/";
			dir ++;
		}
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
