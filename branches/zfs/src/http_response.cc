#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>
#include <list>
#include <map>

bool			net::http::generate_status_line(std::string& ln)
{
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
//	std::list<net::config::mime*>::iterator it;
////	config.get_mimes(it);
//	std::string	ext;
//
//	m_uri.extension(ext);
////	while (config.end_mimes(it) == false)
//	{
//		if (ext == (*it)->extension)
//		{
//			response["Content-type"] = (*it)->type;
//			return true;
//		}
//	}
	response["Content-type"] = "text/html";
	return true;
}

bool	net::http::generate_header_date()
{
	//	struct tm				*newtime;
	char					*datestr;
	std::vector<std::string> v;
	std::string				tmp;
	time_t t;
	struct tm *tb;

	t = time(NULL);
	tb = localtime(&t);
	datestr = ctime(&t);
	datestr[strlen(datestr) - 1] = '\0';
	stringmanager::split(datestr, " ", v);
	tmp = v[0] + ", " + v[2] + " " 
		+ v[1] + " " + v[4] + " " + v[3] + " GMT";
	response["Date"] = tmp;
	return (true);
}

bool			net::http::generate_header_lines(size_t sz, bool chunk)
{
	response["Server"] = "Zfs.";
	generate_header_date();
	generate_content_type();
	if (chunk == false)
		generate_content_length(sz);
	else
		response["Transfer-Encoding"] = "chunked";
	return true;
}

bool			net::http::generate_content_length(size_t sz)
{
	char	t[20];

	sprintf(t, "%i", sz);
	response["Content-length"] = t;
	return true;
}

bool			net::http::create_header(buffer& data, size_t sz, bool chunk)
{
	std::map<std::string, std::string>::iterator iter;
	std::string ln;
	generate_status_line(ln);
	generate_header_lines(sz, chunk);

	data += ln;
	for(iter = response.m_hdrlines.begin(); iter != response.m_hdrlines.end(); iter++)
	{
		data += (*iter).first + ": " + iter->second + "\r\n";
	}
	data += "\r\n";
	return true;
}