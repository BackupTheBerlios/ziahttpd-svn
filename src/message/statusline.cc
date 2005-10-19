
#include <vector>
#include <iostream>
#include <message.hh>
#include <stringmanager.hh>

bool		http::message::statusline_c::method(const std::string &source, int& err)
{
	int		i;

#ifdef _DEBUG
	std::cout << "function method" << std::endl;
#endif
	
	for (i = 0; !http::statusline_key[i].empty(); i++)
	{
		if (http::statusline_key[i] == source)
		{
			this->method_ = i;
			return (true);
		}
	}
#ifdef _DEBUG
	std::cout << "function -method" << std::endl;
#endif
	return (false);
}

bool		http::message::statusline_c::uri(const std::string &str, int& err)
{
	char						*url;
	char						*var = NULL;
	stringmanager::string				parse;
	std::string						tmp(str);

	url = (char *)tmp.c_str();
	uri_ = str;
#ifdef _DEBUG
	std::cout << "function uri" << std::endl;
#endif
	if (url[0] != '/')
	{
		err = 400;
		return(false);
	}
	while (url[0] == '/' && url[1] == '/')
		url++;
	if ((var = strstr(url, "?")))
	{
		*var = '\0';
		var++;
		query_ = var;
	}
	page_ = url;
	if (!parse.unconvert_hexa(page_))
	{
		err = 400;
		return (false);
	}
#ifdef _DEBUG
	std::cout << "page [" << page_ << "]" << std::endl;
#endif
	if (query_.empty())
		return(true);
	std::vector<std::string>	query;


	parse.split(query_, "&", query);

	std::vector<std::string>::iterator theIterator;
	for( theIterator = query.begin(); theIterator != query.end(); theIterator++ )
	{
		std::vector<std::string>	q;
		parse.split(*theIterator, "=", q);
		if (!parse.unconvert_hexa(q[0]))
		{
			err = 400;
			return (false);
		}
		if (!parse.unconvert_hexa(q[1]))
		{
			err = 400;
			return (false);
		}
		variables_[q[0]] = q[1];
		#ifdef _DEBUG
		std::cout << "var [" << q[0] << "][" << q[1]<< "]" << std::endl;
		#endif
	}
	return (true);
}

bool		http::message::statusline_c::version(const std::string &str, int& err)
{
	std::vector<std::string>	q;
	stringmanager::string				parse;
#ifdef _DEBUG
	std::cout << "function version" << std::endl;
#endif
	parse.split(str, "/", q);
	if (q.size() != 2)
	{
		err = 400;
		return (false);
	}
	if (q[0] != "HTTP")
	{
		err = 400;
		return (false);
	}
	version_ = q[1];
	return (true);
}
