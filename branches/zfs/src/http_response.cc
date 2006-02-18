#include <ziafs_resource.hh>
#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>
#include <list>
#include <map>
#include <sys/sysapi.hh>


using namespace sysapi;

bool			net::http::stringify_status_line(std::string& ln)
{
	char				st_code[20];
	std::string	st_code_str;

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

	return true;
}

bool			net::http::generate_content_length(size_t sz)
{
	char	t[20];

	sprintf(t, "%i", sz);
	response["Content-length"] = t;
	return true;
}

bool			net::http::create_header(buffer& data, size_t sz, chunk_pos_t chunk)
{
	data.reset();
	if (chunk == CHUNK_FIRST)
	{
		response["Server"] = "Zfs.";
		generate_header_date();
		generate_content_type();
		if (response.is_chunk == false)
			generate_content_length(sz);
		else
			response["Transfer-Encoding"] = "chunked";
		stringify_header(data);
	}
	response.m_data_enco->encode(data, sz);
	if ((chunk == CHUNK_LAST) && (response.is_chunk == true))
		data += "\r\n";
return true;
}

bool			net::http::modify_header(config& conf)
{
	return true;
}

bool			net::http::get_type_of_resource(net::config& conf, resouce_type_t& type_r)
{
		std::list<net::config::mime*>::iterator it;

		if (m_uri.status_code())
		{
			type_r = IS_FLY;
			return true;
		}
		conf.get_mimes(it);
		std::string	ext;
	
		m_uri.extension(ext);
		while (conf.end_mimes(it) == false)
		{
			if ((ext == (*it)->extension) && (*it)->is_cgi)
			{
				type_r = IS_CGI;
				response.is_chunk = true;
				return true;
			}
			if ((ext == (*it)->extension) && !(*it)->cgi.empty())
			{
				type_r = EXEC_BY_CGI;
				response.is_chunk = true;
				return true;
			}
			it++;
		}
	type_r = IS_FILE;
	return true;
}

bool				net::http::create_resource(resource::handle*& hld, resource::manager& manager, config& conf)
{
	resource::e_error error;

	std::list<net::config::directory*>::iterator	dir;

	hld = 0;
	error = resource::E_SUCCESS;
	//
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
	ziafs_debug_msg("CREATE resource %s", m_uri.localname().c_str());

	resouce_type_t r_type;

	get_type_of_resource(conf, r_type);

	if (r_type == IS_FILE)
		error = manager.factory_create(hld, resource::ID_FILE, resource::O_INPUT, m_uri.localname());
	else if (r_type == IS_CGI)
	{
		int ac = 1;
		const char *tab[] = { m_uri.localname().c_str(), 0};
		const char *env[] = {0};
		error = manager.factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)tab, (char**)env);
	}
	else if (r_type == EXEC_BY_CGI)
	{
		// split du fichier CGI de conf

//		error = manager.factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)tab, (char**)env);
//		error = manager.factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, m_uri.localname());
	}
	else if (r_type == IS_FLY)
		error = manager.factory_create(hld, resource::ID_BYFLY, resource::O_INPUT, m_uri.status_code());

	if (error != resource::E_SUCCESS)
	{
		m_uri.status_code() = 500;
		r_type = IS_FLY;
		error = manager.factory_create(hld, resource::ID_BYFLY, resource::O_INPUT, m_uri.status_code());
	}
	if (!m_uri.status_code())
		m_uri.status_code() = 200;
	if ((r_type == IS_CGI) || (r_type == EXEC_BY_CGI))
		response.m_data_enco = new chunked;
	else
		response.m_data_enco = new unchunked;
	return true;
}

bool			net::http::stringify_header(buffer& data)
{
	std::string ln;
	std::map<std::string, std::string>::iterator iter;

	stringify_status_line(ln);
	data += ln;
	for(iter = response.m_hdrlines.begin(); iter != response.m_hdrlines.end(); iter++)
	{
		data += (*iter).first + ": " + iter->second + "\r\n";
	}
	data += "\r\n";
	return true;
}

status::error		net::http::chunked::encode(buffer& data, size_t sz)
{
	std::string	chunk_hex;

	stringmanager::dec_to_hex((int)sz, chunk_hex);
	data += chunk_hex;
	data += "\r\n";
	ziafs_return_status(status::SUCCESS);
}