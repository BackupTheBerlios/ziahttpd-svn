#include <ziafs_resource.hh>
#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>
#include <list>
#include <map>
#include <sys/sysapi.hh>

using namespace sysapi;

bool			net::http::generate_status_line(std::string& ln)
{
	char				st_code[20];
	std::string	st_code_str;

	if (!m_uri.status_code())
		m_uri.status_code() = 200;
	sprintf(st_code, "%i", m_uri.status_code());
	st_code_str = st_code;
	ln = request.m_version + " " + st_code_str + " ";
	error_code_string(st_code_str);
	ln += st_code_str + "\r\n";
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

	t = ::time(NULL);
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


bool				net::http::create_resource(resource::handle*& hld, resource::manager& manager, config& conf)
{
	resource::e_error error;
	hld = 0;
	std::list<net::config::directory*>::iterator	dir;

	conf.get_directory(dir);
	while (!conf.end_directory(dir))
	{
		if ((*dir)->servername == "*")
			m_uri.localname() = (*dir)->docroot + "/" + m_uri.wwwname();
		if ((*dir)->servername == request["host"])
			m_uri.localname() = (*dir)->docroot + "/" + m_uri.wwwname();
		dir ++;
	}
	
	if (!file::is_readable(m_uri.localname()))
		m_uri.status_code() = 403;
	if (!file::is_path_valid(m_uri.localname()))
		m_uri.status_code() = 404;
	ziafs_debug_msg("CREATE resource %s %i", m_uri.localname().c_str(), m_uri.status_code());
	if (m_uri.status_code() == 0)
		error = manager.factory_create(hld, resource::ID_FILE, resource::O_INPUT, m_uri.localname());
	else
		error = manager.factory_create(hld, resource::ID_BYFLY, resource::O_INPUT, m_uri.status_code());
	if (error != resource::E_SUCCESS)
	{

		return false;
	}
	return true;
}

//bool			net::http::stringify()
//{
//
//}
