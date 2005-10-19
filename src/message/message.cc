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
#include <message.hh>

http::message::message(server::session& s)
{
	session_ = &s;
	error_code_ = -1;
}

bool	http::message::statusline(const std::string& data)
{
	stringmanager::string					parse;
	std::vector<std::string>				glist;
	int										s;

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
	if (!statusline_data.method(glist[0], s))
	{
		error_code_ = s;
		return (false);
	}
	if (!statusline_data.uri(glist[1], s))
	{
		error_code_ = s;
		return (false);
	}
	if (!statusline_data.version(glist[2], s))
	{
		error_code_ = s;
		return (false);
	}
	return (true);
}

bool	http::message::statusline()
{
	return (true);
}


bool	http::message::header(const std::string& data)
{
	char	*var;
	char	*val;
	char	*tmp;

#ifdef _DEBUG
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
	for (int i = 0; !http::header_key[i].empty(); i++)
	{
		if (header_key[i] == var)
		{
			header_data.variables_[var] = val;
#ifdef _DEBUG
			std::cout << "[" << var <<"][" << val << "]" << std::endl;
#endif
			free(var);
			return(true);
		}
	}
	free(var);
	return (true);
}

bool	http::message::header()
{
	return (true);
}

bool	http::message::body(const unsigned char* data, size_t size)
{
	return (true);
}

bool	http::message::body()
{
	return (true);
}
