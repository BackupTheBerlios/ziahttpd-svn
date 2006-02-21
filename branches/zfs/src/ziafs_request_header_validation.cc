#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>

bool	net::http::valid_method()
{
	std::string valid_method[] = {"get", "post", "head", ""};
	std::string method_str(m_method);
	int					flag = 0;

	stringmanager::normalize(method_str);
	for (int i = 0; !valid_method[i].empty(); i++)
	{
		if (valid_method[i] == method_str)
			flag = 1;
	}
	if (!flag)
	{
		// Perhaps a module is using another method
		//m_uri.status_code() = 405;
		//return false;
	}
	return true;
}

bool	net::http::valid_uri()
{
	std::string	host;
	std::string wwwname(m_uri.wwwname());
	std::string tmp;

	if (wwwname[0] != '/')
	{
		host = "http://" + request["host"] + "/";
		if (strncmp(wwwname.c_str(), host.c_str(), host.size()))
		{
			m_uri.status_code() = 400;
			return false;
		}
		tmp = wwwname.substr(host.length() - 1, wwwname.length() - host.length() + 1);
		m_uri.wwwname() = tmp;
	}

	char *str;

	str = (char *)m_uri.wwwname().c_str();
	while ((str[0] == '/') && (str[1] == '/'))
		str++;
	m_uri.wwwname() = str;
	return true;
}

bool	net::http::valid_version()
{
	return true;
}

bool	net::http::valid_host()
{
	return true;
}

bool	net::http::request_header_validation()
{
	if (!valid_method()) return false;
	if (!valid_uri()) return false;
	if (!valid_version()) return false;
	if (!valid_host()) return false;
	return true;
}
