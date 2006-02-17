#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>
#include <list>

bool			net::http::generate_status_line()
{
	std::string	ln;
	char				st_code[20];
	std::string	st_code_str;

	sprintf(st_code, "%i", m_uri.status_code());
	st_code_str = st_code;
	ln = response.m_version + " " + st_code_str + " ";
	error_code_string(st_code_str);
	ln += st_code_str;
	return true;
}

bool			net::http::generate_content_type()
{
	//Nedd config file 
	std::list<net::config::mime*>::iterator it;
//	config.get_mimes(it);
	std::string	ext;

	m_uri.extension(ext);
//	while (config.end_mimes(it) == false)
	{
		if (ext == (*it)->extension)
		{
			response["Content-type"] = (*it)->type;
			return true;
		}
	}
	response["Content-type"] = "text/html";
}

bool			net::http::generate_header_lines()
{
	response["Server"] = "Zfs.";
	return true;
}