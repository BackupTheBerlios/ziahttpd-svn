#include "include/resource.hh"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;
using namespace sysapi;

bool http_helper::generate_chunk_header(buffer& data, size_t sz, chunk_pos_t chunk)
{
  ostringstream oss;

  data.clear();
  oss << hex << (unsigned int)sz;

  data = oss.str() + "\r\n";
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

	//Listing directory 
	if (localname[localname.size() - 1] == '/')
	{
		type_r = EXEC_DIRECTORY_LISTING;
//		response.is_chunk = true;
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
			return true;
		}
		if ((ext == (*it)->extension) && !(*it)->cgi.empty())
		{
			type_r = EXEC_BY_CGI;
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

			tmp = inp.GetInputLocation();
			tmp += "/";
			out.SetOutput("Location", tmp.c_str());
			status_code = 301;
			localname += "/";
		}
	}

	std::string method_str;
	method_str = inp.GetInputMethod();
	stringmanager::normalize(method_str);
	if (method_str != "put")
	{
		if (!file::is_path_valid(localname) && !status_code)
			status_code = 404;
		if (!file::is_readable(localname) && !status_code)
			status_code = 403;
	}
		return true;
}

bool			http_helper::parse_cgi_header(std::string& source, std::string& key, std::string& val)
{
 size_t i;

	i = source.find(':', 0);
	if (i != std::string::npos)
	{
		key = source.substr(0, i);
		val = source.substr(i + 1, source.length() - i - 1);
		stringmanager::remove_space(val);
		stringmanager::normalize(key);
		if (key.empty())
			return false;
	}
	else
	{
		return false;
	}
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

bool				http_helper::genete_type_mimes(net::config& conf, IOutput& out, std::string& localname)
{
	std::list<net::config::mime*>::iterator it;
	conf.get_mimes(it);
	std::string	ext;

	ext = extension(localname);
	while (conf.end_mimes(it) == false)
	{
		if ((*it)->extension == ext)
		{
			out.SetOutput("Content-Type", (*it)->type.c_str());
			return true;
		}
		++it;
	}
	out.SetOutput("Content-Type", "text/html");
	return false;
}

bool				http_helper::generate_cgi_eviron(IInput& inp, IOutput& out, std::string localname, int sz_input, char ***env)
{
	ostringstream	oss;
	std::map<std::string, std::string> m_env_header;
	std::map<std::string, std::string>::iterator it;
	char **ret;

//	env[0] = strdup("AUTH_TYPE=");

	if (sz_input)
	{
		oss << sz_input;
		m_env_header["CONTENT_LENGTH"] = oss.str();
		oss.str("");
	}
	
	oss << out.GetOutput("content-type");
	m_env_header["CONTENT_TYPE"] = oss.str();
	oss.str("");

////	m_env_header["GATEWAY_INTERFACE"] = "CGI/1.1";

	oss << inp.GetInputLocation();
	m_env_header["PATH_INFO"] = oss.str();
	oss.str("");

	oss << localname;
	m_env_header["PATH_TRANSLATED"] = oss.str();
	oss.str("");

	if (inp.GetInputQueryString())
	{
		oss << inp.GetInputQueryString();
		m_env_header["QUERY_STRING"] = oss.str();
	}
	else
		m_env_header["QUERY_STRING"] = "";
	oss.str("");

	char *tmp;
	int ip;
	ip = inp.GetClientIp();
	tmp = (char *)&ip;
	for (int i = 0; i < 4; i++)
	{
		int o = (int)*tmp;
		oss << o;
		if (i != 3)
			oss << ".";
		tmp++;
	}
	m_env_header["REMOTE_ADDR"] = oss.str();
	oss.str("");
	m_env_header["REMOTE_HOST"] = "";

//	env[9] = strdup("REMOTE_IDENT=");
	
//	env[10] = strdup("REMOTE_USER=");
	
	////oss << inp.GetInputMethod();
	////m_env_header["REQUEST_METHOD"] = oss.str();
	////oss.str("");

	char *script = (char *)inp.GetInputLocation();
	script += strlen (script) - 1;
	while (*script != '/')
		script --;
	script ++;
	oss << script;
	m_env_header["SCRIPT_NAME"] = oss.str();
	oss.str("");

	//m_env_header["SERVER_NAME"] = "Zfs.";
	
//	env[14] = strdup("SERVER_PORT=");
	
	oss << inp.GetInputHttpVersion();
	m_env_header["SERVER_PROTOCOL"] = oss.str();
	oss.str("");

//	env[16] = strdup("SERVER_SOFTWARE=");
	
	ret = new char*[m_env_header.size()];
	int y = 0;
	for (it = m_env_header.begin(); it != m_env_header.end(); it++)
	{
		string stmp;

		if (!it->second.empty())
		{
			stmp = it->first + "=" + it->second;
			ret[y] = strdup(stmp.c_str());
			y++;
		}
	}
	ret[y] = 0;
	*env = ret;
	return true;

}
bool				http_helper::free_cgi_environ(char **env)
{
	int i;

	for (i = 0; env[i]; i++)
		free(env[i]);
	delete [] env;
	return true;
}

bool				http_helper::create_resource(resource::handle*& hld, 
																				 net::config& conf, IInput& inp, 
																				 IOutput& out, 
																				 std::string localname)
{
	resource::e_error error = resource::E_SUCCESS;
	resouce_type_t r_type = IS_NONE;
	int status_code = 0;
	  unsigned int sz_input;

	pre_create_resource(conf, r_type, status_code, inp, out, localname);
	get_type_of_resource(conf, r_type, inp, out, localname, status_code);
	sz_input = 0;
	if (inp.GetInput("content-length"))
	  sz_input = atoi(inp.GetInput("Content-length"));

	if (r_type == IS_FILE)
	{
		genete_type_mimes(conf, out, localname);
		error = resource::manager::factory_create(hld, resource::ID_FILE, resource::O_INPUT, localname);
	}
	else if (r_type == IS_CGI)
	{
		int ac = 1;
		const char *tab[] = { localname.c_str(), 0};
		// This line has been modified by texane
		// buffer is a buffer to prefecth the input.
		// content of the mline.buf will be removed
		char **env;
		generate_cgi_eviron(inp, out, localname, sz_input,&env);
	  error = resource::manager::factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)tab, (char**)env, sz_input);
		free_cgi_environ(env);
	}
	else if (r_type == EXEC_BY_CGI)
	{
		std::string cgi_path;
		char **av;
		char **env;
		//char *env[] = {"SERVER_PORT=80", 0};
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

		generate_cgi_eviron(inp, out, localname, sz_input, &env);
		error = resource::manager::factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)av, (char**)env, sz_input);
		free_cgi_environ(env);
		delete [] av;
	}
	else if (r_type == EXEC_DIRECTORY_LISTING)
	{
		std::string cgi_path;
		char **av;
		char **env;
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

		generate_cgi_eviron(inp, out, localname, sz_input, &env);
		error = resource::manager::factory_create(hld, resource::ID_PROCESS, resource::O_BOTH, ac, (char**)av, (char**)env);
		free_cgi_environ((char **)env);
	}
	else if (r_type == IS_PUT)
	  error = resource::manager::factory_create(hld, resource::ID_FILE, resource::O_OUTPUT, localname, sz_input);
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

resource::handle* http_helper::create_resource(IInput& in, const char* path, IOutput& out, net::config* conf)
{

	resource::handle *hdl;

	create_resource(hdl, *conf, in, out, path);
  return hdl;
}
