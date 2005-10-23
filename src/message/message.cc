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
#include <session.hh>

http::message::message(server::session* s)
{
	session_ = s;
	error_code_ = 200;
	page_is_dir = false;
}

bool	http::message::statusline(const char* str)
{
	stringmanager::string					parse;
	stringmanager::httpsm					http_par;
	std::vector<std::string>				glist;
	std::string								data;


	std::cout << "Original client status line : [" << &str << "]" << std::endl; 
	if (!strlen(str))
		return (false);
	data = str;
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

	if (method_ == "get")
		session_->http_info_.is_method_get_ = true;
	if (method_ == "post")
		session_->http_info_.is_method_post_ = true;
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
			header_[var] = val;
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
	header_[var] = val;
	free(var);
	return (true);
}

bool	http::message::body(const unsigned char* data, sysapi::socket_in::size_t size)
{
	std::cout << "ERR CODE " << error_code_ << std::endl;
	if (method_ == "post")
	{
		stringmanager::httpsm	http_par;
		std::string				s((char *)data);
		int						si = -1;

		si = atoi(header_["content-length"].c_str());
		if (si)
			s = s.substr(0, si);
		if (!http_par.query(s, postquery_, error_code_))
			return (false);
	}
	if (method_ == "put")
	{
		// not yet implemented
	}

	return (true);
}
