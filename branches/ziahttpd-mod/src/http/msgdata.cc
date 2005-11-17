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


bool	http::msgdata::parse_rqstline(buffer& buf, uri& uri)
{
	static	bool	status_line = true;

	if (!status_line)
		return (parse_rqstline_statusline(buf, uri));
	else
		return (parse_rqstline_headerline(buf, uri));
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
	*str++ = '\0';
	p.normalize(tmp);
	hdrlines_[tmp] = str;
	return (true);
}

bool	http::msgdata::parse_rqstline_headerline(buffer& buf, uri& uri)
{
	stringmanager::string	p;
	unsigned char			*str;
	std::string				s;
	std::vector<std::string>		vec;

	str = (const_cast<dataman::buffer&>(buf));
	s = (char *)str;
	p.split(s, " ", vec);
	if (vec.size() != 3)
		return (false);
	// method
	method_ = p.normalize(vec[0]);
	// parse uri
	parse_uri(vec[1]);
	// http version
	version_ = p.normalize(vec[2]);
	return (true);
}

bool	http::msgdata::parse_uri(std::string& uri)
{
	char	*tmp;
	char	*str;
	int		flag;
	stringmanager::string	p;

	tmp = strdup(uri.c_str());
	str = tmp;
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
	uri_ = str;
	p.unconvert_hexa(uri_);
	if (flag)
		query_ = ++tmp;
	p.unconvert_hexa(query_);
	free(str);
	return (true);
}

bool	http::msgdata::stringify_respline(buffer& metadata)
{

	return (true);
}