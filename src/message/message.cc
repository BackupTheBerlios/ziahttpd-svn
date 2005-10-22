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
		{"content-length", response_header_content_length},
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

bool	http::message::body(const unsigned char* data, size_t size)
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

bool	http::message::error_code_string(std::string &dest)
{
	struct	err_list_s
	{
		int			err_code;
		std::string str;
	};
	err_list_s	err_list[] = 
	{
		{100, "Continue"},	
		{101, "Switching Protocols"},
		{200, "OK"},
		{201, "Created"},
		{202, "Accepted"},
		{203, "Non-Authoritative Information"},
		{204, "No Content"},
		{205, "Reset Content"},
		{206, "Partial Content"},
		{300, "Multiple Choices"},
		{301, "Moved Permanently"},
		{302, "Found"},
		{303, "See Other"},
		{304, "Not Modified"},
		{305, "Use Proxy"},
		{307, "Temporary Redirect"},
		{400, "Bad Request"},
		{401, "Unauthorized"},
		{402, "Payment Required"},
		{403, "Forbidden"},
		{404, "Not Found"},
		{405, "Method Not Allowed"},
		{406, "Not Acceptable"},
		{407, "Proxy Authentication Required"},
		{408, "Request Time-out"},
		{409, "Conflict"},
		{410, "Gone"},
		{411, "Length Required"},
		{412, "Precondition Failed"},
		{413, "Request Entity Too Large"},
		{414, "Request-URI Too Large"},
		{415, "Unsupported Media Type"},
		{416, "Requested range not satisfiable"},
		{417, "Expectation Failed"},
		{500, "Internal Server Error"},
		{501, "Not Implemented"},
		{502, "Bad Gateway"},
		{503, "Service Unavailable"},
		{504, "Gateway Time-out"},
		{505, "HTTP Version not supported"},
		{0, ""}
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
