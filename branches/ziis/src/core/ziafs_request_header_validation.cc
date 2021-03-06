#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>


#ifndef _WIN32
extern int stricmp(const char*, const char*);
extern int strnicmp(const char*, const char*, int);
#endif // ! _WIN32

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

	if (!strncmp("http://", wwwname.c_str(), 7))
	{
		tmp = wwwname.substr(7, wwwname.length());

		size_t i = tmp.find("/", 0);
		if (i != std::string::npos)
		{
			std::string tmmp;

			tmmp = tmp.substr(i, tmp.length() - i);
			tmp = tmmp;
		}
		else
		{
			tmp = "/";
		}
		m_uri.wwwname() = tmp;
	}
	//if (wwwname[0] != '/')
	//{
//		host = "http://";
		//if (strncmp(wwwname.c_str(), host.c_str(), host.size()))
		//{
		//	m_uri.status_code() = 400;
		//	return false;
		//}
		
		
	//}

	char *str;

	str = (char *)m_uri.wwwname().c_str();
	while ((str[0] == '/') && (str[1] == '/'))
		str++;
	m_uri.wwwname() = str;
	if (m_uri.wwwname().empty() || (m_uri.wwwname()[0] != '/'))
	{
		m_uri.status_code() = 400;
		return false;
	}
	return true;
}

bool	net::http::valid_version()
{
	
	if (!strnicmp("http/", request.m_version.c_str(), 5))
	{
		if (stricmp("http/1.1", request.m_version.c_str()) && stricmp("http/1.0", request.m_version.c_str()))
		{
			m_uri.status_code() = 505;
			return false;
		}
	}
	else
	{
		m_uri.status_code() = 501;
		return false;
	}
	return true;
}

bool	net::http::valid_host()
{
	if ((!stricmp("http/1.1", request.m_version.c_str())) && (request["host"].empty()))
	{
		m_uri.status_code() = 400;
		return false;
	}
	return true;
}

bool	net::http::valid_root()
{
	// root is valid ?
	std::vector<std::string>	vec;
	std::string str;
	str = m_uri.wwwname();
	str += "lala";
	stringmanager::split(str, "/", vec);
	std::vector<std::string>::iterator iter;
	int			i = 0;
	for(iter = vec.begin(); iter != vec.end(); iter++)
	{
		if ((*iter) == "..")
			i--;
		else
			i++;
	}
	i--;
	if (i < 0)
	{
		m_uri.status_code() = 403;
		return false;
	}
	return true;
}

bool	net::http::request_header_validation()
{
	if (!valid_method()) return false;
	if (!valid_uri()) return false;
	if (!valid_version()) return false;
	if (!valid_host()) return false;
	if (!valid_root()) return false;
	return true;
}
