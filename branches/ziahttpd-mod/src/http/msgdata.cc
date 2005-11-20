//
// msgdata.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:56:50 2005 
// Last update Sun Nov 13 16:05:39 2005 
//


#include <string>
#include <http/uri.hh>
#include <http/msgdata.hh>
#include <dataman/buffer.hh>
#include <vector>
#include <dataman/stringmanager.hh>
#include <stdio.h>
#include <iostream>

using std::string;

using dataman::buffer;


http::msgdata::~msgdata()
{

}


string&	http::msgdata::operator[](const string& key)
{
	string str(key);
	stringmanager::string	p;

	p.normalize(str);
	return (hdrlines_[str]);
}

string&	http::msgdata::operator=(const string& val)
{
	string t(val);
	return (t);
}

bool	http::msgdata::parse_rqstline(buffer& buf, uri& uri)
{
	static	bool	status_line = true;

	if (!status_line)
	{
		status_line = false;
		return (parse_rqstline_statusline(buf, uri));
	}
	else
	{
		status_line = false;
		return (parse_rqstline_headerline(buf, uri));
	}
	return false;
}


bool	http::msgdata::build_respline(msgdata& rqst, const uri&)
{

	return (false);
}


string&	http::msgdata::query_string()
{
  return (query_);
}

bool	http::msgdata::parse_rqstline_statusline(buffer& buf, uri& uri)
{
	char *str;
	char *tmp;
	stringmanager::string	p;

	str = buf.c_str();
	tmp = str;
	while (*str)
	{
		if (*str == ':')
			break ;
		str++;
	}
	if (!*str)
		return (false);
	*str++ = '\0';
	while (*str == ' ')
		str++;
	p.normalize(tmp);
	hdrlines_[tmp] = str;
	return (true);
}

bool	http::msgdata::parse_rqstline_headerline(buffer& buf, uri& uri)
{
	stringmanager::string			p;
	char							*str;
	std::string						s;
	std::vector<std::string>		vec;

	str = buf.c_str();
	s = (char *)str;
	p.split(s, " ", vec);
	if (vec.size() != 3)
	{
		free(str);
		return (false);
	}
	// method
	method_ = vec[0];
	p.normalize(method_);
	// parse uri
	if (!parse_uri(vec[1]))
		return (false);
	// http version
	version_ = vec[2];
	p.normalize(version_);
	if (strncmp(version_.c_str(), "http/", 5))
	{
		version_ = "http/1.1";
		return (false);
	}
	free(str);
	return (true);
}

bool	http::msgdata::parse_uri(std::string& uri)
{
	char	*tmp;
	char	*tmp2;
	char	*str;
	int		flag;
	stringmanager::string	p;

	tmp = strdup(uri.c_str());
	str = tmp;

	if (tmp[0] != '/')
		return (false);
	while (tmp[0] == '/' && tmp[1] == '/')
		tmp++;
	tmp2 = tmp;
	flag = 0;
	while (*tmp)
	{
		if (*tmp == '?')
		{
			flag = 1;
			break ;
		}
		tmp++;
	}
	*tmp = '\0';
	uri_ = tmp2;
	p.unconvert_hexa(uri_);
	if (flag)
		query_ = ++tmp;
	p.unconvert_hexa(query_);
	free(str);
	return (true);
}

bool	http::msgdata::stringify_respline(buffer& metadata, uri& u)
{
	char sta[4];
	std::map<std::string, std::string>::iterator iter;

	sprintf(sta, "%i", u.status());

	metadata = version_ 
			+ " "
			+ sta
			+ " " 
			+ u.strstatus() 
			+ "\r\n";
	for(iter = hdrlines_.begin(); iter != hdrlines_.end(); iter++)
	{
		metadata += iter->first + ": " + iter->second + "\r\n";
	}
	metadata += "\r\n";
	return (true);
}

http::msgdata& http::msgdata::operator=(http::msgdata& msg)
{
	this->method_ = msg.method_string();
	version_ = msg.version_string();
	uri_ = msg.uri_string();
	query_ = msg.query_string();
	return (*this);
}