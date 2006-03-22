#include "include/resource.hh"


using namespace std;
using namespace sysapi;

bool http_helper::generate_chunk_header(buffer& data, size_t sz, chunk_pos_t chunk)
{
  ostringstream oss;

  data.clear();
  oss << hex << (unsigned int)sz;

  if (chunk != CHUNK_FIRST)
    data += "\r\n" + oss.str();
  else
    data += oss.str();
  data += "\r\n";
  if ((chunk == CHUNK_LAST))
    data += "\r\n";
  return true;
}

std::string	http_helper::extension(std::string& localname)
{
	std::string ret;

	size_t i = localname.rfind(".", localname.length() - 1);
	if (i != std::string::npos)
	{
		ret = localname.substr(i + 1, localname.length() - i - 1);
	}
	return ret;
}

bool			http_helper::get_type_of_resource(net::config& conf, 
																						resouce_type_t& type_r, 
																						IInput& inp, 
																						IOutput& out, 
																						std::string& localname,
																						int& status_code)
{
	std::list<net::config::mime*>::iterator it;
	std::string method_str(inp.GetInputMethod());
	stringmanager::normalize(method_str);

	if (status_code)
	{
		type_r = IS_FLY;
		return true;
	}
	//Listing directory 
	if (localname[localname.size() - 1] == '/')
	{
		type_r = EXEC_DIRECTORY_LISTING;
//		response.is_chunk = true;
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

	ext = extension(localname);
	while (conf.end_mimes(it) == false)
	{
		if ((ext == (*it)->extension) && (*it)->is_cgi)
		{
			type_r = IS_CGI;
//			response.is_chunk = true;
			return true;
		}
		if ((ext == (*it)->extension) && !(*it)->cgi.empty())
		{
			type_r = EXEC_BY_CGI;
//			response.is_chunk = true;
			return true;
		}
		it++;
	}
	type_r = IS_FILE;
	return true;
}

bool				http_helper::pre_create_resource(net::config& conf,
																							resouce_type_t& r_type,
																							int& status_code,
																							IInput& inp,
																							IOutput& out,
																							std::string& localname)
{
	std::list<net::config::directory*>::iterator	dir;
	std::list<net::config::server*>::iterator			serv;
	std::vector<std::string>::iterator						dir_index;
	std::string																		doc_root;

	if (localname[localname.size() - 1] != '/')
	{
		// Move if directory
		if (file::is_directory(localname))
		{
			std::string tmp;

			tmp = "http://";
			tmp += inp.GetInput("host");
			tmp += inp.GetInputLocation();
			tmp += "/";
			out.SetOutput("Location", tmp.c_str());
			status_code = 301;
			localname += "/";
		}
	}

	if (!file::is_path_valid(localname) && !status_code)
		status_code = 404;
	if (!file::is_readable(localname) && !status_code)
		status_code = 403;
	return true;
}

bool			http_helper::get_cgi_path(net::config& conf, std::string& path, std::string& localname)
{
	//Need config file 
	std::list<net::config::mime*>::iterator it;
	conf.get_mimes(it);
	std::string	ext;

	ext = extension(localname);
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

bool				http_helper::create_resource(resource::handle*& hld, 
																				 net::config& conf, IInput& inp, 
																				 IOutput& out, 
																				 std::string localname)
{
	resource::e_error error = resource::E_SUCCESS;
	resouce_type_t r_type = IS_NONE;
	int status_code = 0;

//	ziafs_debug_msg("CREATE resource %s", m_uri.localname().c_str());
	pre_create_resource(conf, r_type, status_code, inp, out, localname);

	get_type_of_resource(conf, r_type, inp, out, localname, status_code);

	if (r_type == IS_FILE)
		error = resource::manager::factory_create(hld, resource::ID_FILE, resource::O_INPUT, localname);
	else if (r_type == IS_CGI)
	{
		int ac = 1;
		const char *tab[] = { localname.c_str(), 0};
		const char *env[] = {0};
		// This line has been modified by texane
		// buffer is a buffer to prefecth the input.
		// content of the mline.buf will be removed
	  error = resource::manager::factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)tab, (char**)env);
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

		get_cgi_path(conf, cgi_path, localname);

		// split du fichier CGI de conf
		stringmanager::split(cgi_path, " ", vec);
		ac = (int)vec.size() + 1;
		av = new char*[vec.size() + 2];
		for(iter = vec.begin(), i = 0; iter != vec.end(); iter++, i++)
		{
			av[i] = (char *)(*iter).c_str();
		}
		av[i++] = (char *)localname.c_str();
		av[i] = '\0';

		error = resource::manager::factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)av, (char**)env);
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
		av[i++] = (char *)localname.c_str();
		av[i] = '\0';
		//std::string cgi_path;
		//char **av;
		//int ac;

		const char *tab[] = { conf.get_system()->directory_listing.c_str() , localname.c_str(), 0};
		//const char *env[] = {0};

		error = resource::manager::factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)av, (char**)env);

	}
	//else if (r_type == IS_PUT) ;
	//		error = manager.factory_create(hld, resource::ID_FILE, resource::O_OUTPUT, m_uri.localname(), &m_line.m_buf, body_size());
	else if (r_type == IS_FLY)
		error = resource::manager::factory_create(hld, resource::ID_BYFLY, resource::O_INPUT, status_code);
	else if (r_type == IS_FAKE)
		error = resource::manager::factory_create(hld, resource::ID_FAKE);

	if (error != resource::E_SUCCESS)
	{
		status_code = 500;
		r_type = IS_FLY;
		error = resource::manager::factory_create(hld, resource::ID_BYFLY, resource::O_INPUT, status_code);
	}
	if (!status_code)
		status_code = 200;
	out.SetStatusCode(status_code);
	return true;
}

resource::handle* http_helper::create_resource(IInput& in, const char* path, IOutput& out)
{
	net::config conf("conf/ziafs.xml");
	resource::handle *hdl;

	create_resource(hdl, conf, in, out, path);
  return hdl;
}
