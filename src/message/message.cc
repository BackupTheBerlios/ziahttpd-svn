/********************************************************************
	created:	2005/10/13
	created:	13:10:2005   17:36
	filename: 	e:\work\c++\zia3\message.cc
	file path:	e:\work\c++\zia3
	file base:	message
	file ext:	cc
	author:		
	
	purpose:	
*********************************************************************/

#include <iostream>
#include <vector>
#include <stringmanager.hh>
#include <httpstringmanager.hh>
#include <message.hh>

http::message::message(server::session* s)
{
	session_ = s;
	error_code_ = 200;
	page_is_dir = false;
}

bool	http::message::statusline(const std::string& data)
{
	stringmanager::string					parse;
	stringmanager::httpsm					http_par;
	std::vector<std::string>				glist;

	parse.split(data," ", glist);
#ifdef _DEBUG
	std::vector<std::string>::iterator theIterator;
	for( theIterator = glist.begin(); theIterator != glist.end(); theIterator++ )
	{
		std::cout << "[" << *theIterator << "]"<< std::endl;
	}
#endif
	if (glist.size() != 3)
	{
		error_code_ = 400;
		return (false);
	}
	if (!http_par.method(glist[0], method_, error_code_))
		return (false);
	if (!uri(glist[1], error_code_))
		return (false);
	if (!http_par.version(glist[2], version_, error_code_))
		return (false);
	return (true);
}

bool	http::message::header(const std::string& data)
{
	char	*var;
	char	*val;
	char	*tmp;
	stringmanager::string	p;
	struct	header_list_s
	{
		std::string	var;
		bool (*fct_eq)(server::session*, const std::string&);
	};

	header_list_s	header_list[] =
	{
		{"host", NULL},
		{"content-length", request_header_content_length},
		{"", NULL}
	};

#ifdef _debug
	std::cout << "function header" << std::endl;
#endif
	var = strdup(data.c_str());
	val = var;
	tmp = val;
	while (*tmp)
	{
		if (*tmp == ':')
			break ;
		tmp++;
	}
	if (!*tmp)
	{
		error_code_ = 400;
		free(val);
		return (false);
	}
	*tmp = '\0';
	val = tmp + 1;
	while (*val == ' ')
		val++;
	p.normalize(var);
	for (int i = 0; !header_list[i].var.empty(); i++)
	{
		if (header_list[i].var == var)
		{
			if (header_list[i].fct_eq)
				header_list[i].fct_eq(session_, val);
//			header_[var] = val;
#ifdef _debug
			std::cout << "[" << var <<"][" << val << "]" << std::endl;
#endif
			free(var);
			return(true);
		}
	}
#ifdef _debug
	std::cout << "uk [" << var <<"][" << val << "]" << std::endl;
#endif
//	header_[var] = val;
	free(var);
	return (true);
}

//bool	http::message::header()
//{
////	std::cout << "Content-Type: text/html" << std::endl;
//	return (true);
//}

bool	http::message::body(const unsigned char* data, sysapi::socket_in::size_t size)
{
	std::cout << "ERR CODE " << error_code_ << std::endl;
	if (method_ == "post")
	{
		stringmanager::httpsm	http_par;
		std::string				s((char *)data);

		if (!http_par.query(s, postquery_, error_code_))
			return (false);
	}
	if (method_ == "put")
	{
		// not yet implemented
	}

	return (true);
}

//bool	http::message::body()
//{
//	//sysapi::file::handle_t	*fd = NULL;
//
//	//if (!sysapi::file::open(fd, path_.c_str(), sysapi::file::RDONLY))
//	//	error_code_ = 404;
//	//sysapi::file::close(*fd);
//	return (true);
//}

bool	http::message::error_code_string(std::string &dest, std::string &dest_file)
{
	struct	err_list_s
	{
		int			err_code;
		std::string str;
		std::string file;
	};
	err_list_s	err_list[] = 
	{
		{100, "Continue", "error.html"},	
		{101, "Switching Protocols", "error.html"},
		{200, "OK", "error.html"},
		{201, "Created", "error.html"},
		{202, "Accepted", "error.html"},
		{203, "Non-Authoritative Information", "error.html"},
		{204, "No Content", "error.html"},
		{205, "Reset Content", "error.html"},
		{206, "Partial Content", "error.html"},
		{300, "Multiple Choices", "error.html"},
		{301, "Moved Permanently", "error.html"},
		{302, "Found", "error.html"},
		{303, "See Other", "error.html"},
		{304, "Not Modified", "error.html"},
		{305, "Use Proxy", "error.html"},
		{307, "Temporary Redirect", "error.html"},
		{400, "Bad Request", "error.html"},
		{401, "Unauthorized", "error.html"},
		{402, "Payment Required", "error.html"},
		{403, "Forbidden", "error.html"},
		{404, "Not Found", "error.html"},
		{405, "Method Not Allowed", "error.html"},
		{406, "Not Acceptable", "error.html"},
		{407, "Proxy Authentication Required", "error.html"},
		{408, "Request Time-out", "error.html"},
		{409, "Conflict", "error.html"},
		{410, "Gone", "error.html"},
		{411, "Length Required", "error.html"},
		{412, "Precondition Failed", "error.html"},
		{413, "Request Entity Too Large", "error.html"},
		{414, "Request-URI Too Large", "error.html"},
		{415, "Unsupported Media Type", "error.html"},
		{416, "Requested range not satisfiable", "error.html"},
		{417, "Expectation Failed", "error.html"},
		{500, "Internal Server Error", "error.html"},
		{501, "Not Implemented", "error.html"},
		{502, "Bad Gateway", "error.html"},
		{503, "Service Unavailable", "error.html"},
		{504, "Gateway Time-out", "error.html"},
		{505, "HTTP Version not supported", "error.html"},
		{0, "", ""}
	};
	for (int i = 0; err_list[i].err_code; i++)
	{
		if (err_list[i].err_code == error_code_)
		{
			dest = err_list[i].str;
			return (true);
		}
	}
	return (false);
}
