///#include <ziafs_resource.hh>
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

bool			net::http::generate_content_type(config& conf)
{
	//Need config file 
	std::list<net::config::mime*>::iterator it;
	conf.get_mimes(it);
	std::string	ext;

	m_uri.extension(ext);
	while (conf.end_mimes(it) == false)
	{
		if (ext == (*it)->extension)
		{
			response["Content-type"] = (*it)->type;
			return true;
		}
		it++;
	}
	response["Content-type"] = "text/html";
	return true;
}

bool			net::http::get_cgi_path(config& conf, std::string& path)
{
	//Need config file 
	std::list<net::config::mime*>::iterator it;
	conf.get_mimes(it);
	std::string	ext;

	m_uri.extension(ext);
	while (conf.end_mimes(it) == false)
	{
		if (ext == (*it)->extension)
		{
			path = (*it)->cgi;
			return true;
		}
		it++;
	}
	path = "";
	//response["Content-type"] = "text/html";
	return false;
}


bool	net::http::generate_header_date()
{
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
	//response["Content-Lenght"] = t;
	response["Content-Length"] = t;
	return true;
}

bool			net::http::create_header(buffer& data, size_t sz, chunk_pos_t chunk)
{
	data.reset();
	if (chunk == CHUNK_FIRST)
	{
		response["Server"] = "Zfs.";

		response["Content-type"] = "text/html";
		generate_header_date();
		if (response.is_chunk == false)
			generate_content_length(sz);
		else
			response["Transfer-Encoding"] = "chunked";
//////////////////////////////////////////////////////////////////////////
		//		modify_header();
//////////////////////////////////////////////////////////////////////////
		stringify_header(data);
	}
	response.m_data_enco->encode(data, sz, chunk);
	if ((chunk == CHUNK_LAST) && (response.is_chunk == true))
		data += "\r\n";
	return true;
}

bool			net::http::create_header()
{
	if (response["Transfer-Encoding"] == "chunked")
		response.m_data_enco = new chunked;
	else
		response.m_data_enco = new unchunked;
	response["Server"] = "Zfs.";
	response["Content-type"] = "text/html";
	generate_header_date();

	return true;
}

bool			net::http::modify_header(config& conf, chunk_pos_t chunk)
{
	generate_content_type(conf);
	return true;
}

bool			net::http::get_type_of_resource(net::config& conf, resouce_type_t& type_r)
{
		std::list<net::config::mime*>::iterator it;
		std::string method_str(m_method);
		stringmanager::normalize(method_str);

		if (m_uri.status_code())
		{
			type_r = IS_FLY;
			return true;
		}
		//Listing directory 
		if (m_uri.wwwname()[m_uri.wwwname().size() - 1] == '/')
		{
			type_r = EXEC_DIRECTORY_LISTING;
			response.is_chunk = true;
			return true;
		}

		if (method_str == "head")
		{
			type_r = IS_FAKE;
			return true;
		}

		if (method_str == "put")
		{
			type_r = IS_PUT;
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

bool				net::http::pre_create_resource(net::config& conf, resouce_type_t& r_type)
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
		if ((*dir)->servername == request["host"])
			doc_root = (*dir)->docroot + "/";
		dir ++;
	}

	if (m_uri.wwwname()[m_uri.wwwname().size() - 1] == '/')
	{
		// check directory listing
		int	have_dir = 0;
		conf.get_server(serv);
		for (dir_index = (*serv)->directory_index.begin(); dir_index != (*serv)->directory_index.end(); dir_index++)
		{
			std::string	tmp;

			tmp = doc_root + m_uri.wwwname() + (*dir_index);
			if (file::is_readable(tmp))
			{
				m_uri.wwwname() += (*dir_index);
				have_dir = 1;
				break;
			}
		}
		
		//Listing directory 
		//if (!have_dir)
		//{
		//	r_type = EXEC_DIRECTORY_LISTING;
		//	response.is_chunk = true;
		//}
	}
	m_uri.localname() = doc_root + m_uri.wwwname();

	if (m_uri.wwwname()[m_uri.wwwname().size() - 1] != '/')
	{
		// Move if directory
		if (file::is_directory(m_uri.localname()))
		{
			response["Location"] = "http://" + request["host"] + m_uri.wwwname() + "/";
			m_uri.status_code() = 301;
		}
	}

	if (!file::is_path_valid(m_uri.localname()) && !m_uri.status_code())
		m_uri.status_code() = 404;
	if (!file::is_readable(m_uri.localname()) && !m_uri.status_code())
		m_uri.status_code() = 403;
	return true;
}

bool				net::http::create_resource(resource::handle*& hld, resource::manager& manager, config& conf)
{
///	resource::e_error error = resource::E_SUCCESS;
	resouce_type_t r_type = IS_NONE;

	ziafs_debug_msg("CREATE resource %s", m_uri.localname().c_str());
	pre_create_resource(conf, r_type);

	get_type_of_resource(conf, r_type);

	if (r_type == IS_FILE)
		;
///		error = manager.factory_create(hld, resource::ID_FILE, resource::O_INPUT, m_uri.localname());
	else if (r_type == IS_CGI)
	{
		int ac = 1;
		const char *tab[] = { m_uri.localname().c_str(), 0};
		const char *env[] = {0};
		  // This line has been modified by texane
		  // buffer is a buffer to prefecth the input.
		  // content of the mline.buf will be removed
///		  error = manager.factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)tab, (char**)env, &m_line.m_buf, body_size());
	}
	else if (r_type == EXEC_BY_CGI)
	{
		std::string cgi_path;
		char **av;
		const char *env[] = {"SERVER_PORT=80", 0};
		int ac;
		std::vector<std::string> vec;
		std::vector<std::string>::iterator iter;
		int i;

		get_cgi_path(conf, cgi_path);

		// split du fichier CGI de conf
		stringmanager::split(cgi_path, " ", vec);
		ac = (int)vec.size() + 1;
		av = new char*[vec.size() + 2];
		for(iter = vec.begin(), i = 0; iter != vec.end(); iter++, i++)
		{
			av[i] = (char *)(*iter).c_str();
		}
		av[i++] = (char *)m_uri.localname().c_str();
		av[i] = '\0';

///		  error = manager.factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)av, (char**)env, &m_line.m_buf, body_size());
		// free env + av
		//		for (i = 0; av[i]; i++)
		//			delete av[i];
		delete [] av;
	}
	else if (r_type == EXEC_DIRECTORY_LISTING)
	{
		std::string cgi_path;
		char **av;
		const char *env[] = {"SERVER_PORT=80", 0};
		int ac;
		std::vector<std::string> vec;
		std::vector<std::string>::iterator iter;
		int i;

		cgi_path = conf.get_system()->directory_listing.c_str();

		// split du fichier CGI de conf
		stringmanager::split(cgi_path, " ", vec);
		ac = (int)vec.size() + 1;
		av = new char*[vec.size() + 2];
		for(iter = vec.begin(), i = 0; iter != vec.end(); iter++, i++)
		{
			av[i] = (char *)(*iter).c_str();
		}
		av[i++] = (char *)m_uri.localname().c_str();
		av[i] = '\0';
		//std::string cgi_path;
		//char **av;
		//int ac;

		////const char *tab[] = { conf.get_system()->directory_listing.c_str() , m_uri.localname().c_str(), 0};
		//const char *env[] = {0};

///		error = manager.factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)av, (char**)env);

	}
///	else if (r_type == IS_PUT) ;
///		error = manager.factory_create(hld, resource::ID_FILE, resource::O_OUTPUT, m_uri.localname(), &m_line.m_buf, body_size());
///	else if (r_type == IS_FLY) ;
///		error = manager.factory_create(hld, resource::ID_BYFLY, resource::O_INPUT, m_uri.status_code());
///	else if (r_type == IS_FAKE) ;
///		error = manager.factory_create(hld, resource::ID_FAKE);

///	if (error != resource::E_SUCCESS)
	{
		m_uri.status_code() = 500;
		r_type = IS_FLY;
///		error = manager.factory_create(hld, resource::ID_BYFLY, resource::O_INPUT, m_uri.status_code());
	}
	if (!m_uri.status_code())
		m_uri.status_code() = 200;
	//if ((r_type == IS_CGI) || (r_type == EXEC_BY_CGI) || (r_type == EXEC_DIRECTORY_LISTING))
	//	response.m_data_enco = new chunked;
	//else
	//	response.m_data_enco = new unchunked;
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

status::error		net::http::chunked::encode(buffer& data, size_t sz, chunk_pos_t chunk)
{
	std::string	chunk_hex;

	stringmanager::dec_to_hex((int)sz, chunk_hex);
	if (chunk != net::CHUNK_FIRST)
		data += "\r\n" + chunk_hex;
	else
		data += chunk_hex;
	data += "\r\n";
	ziafs_return_status(status::SUCCESS);
}

