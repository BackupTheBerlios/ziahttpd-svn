//
// uri.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:51:23 2005 
// Last update Sun Nov 13 17:44:26 2005 
//


#include <string>
#include <http/session.hh>
#include <http/uri.hh>


using std::string;


http::uri::uri()
{
	status_ = 200;
}


http::uri::uri(session& s)
{

}

void	http::uri::reset()
{
	status_ = 200;
	strstatus_ = "";
	localname_ = "";
	widename_ = "";
	extension_ = "";
}

int&	http::uri::status()
{
  return status_;
}

string&	http::uri::strstatus()
{
  return strstatus_;
}



string&	http::uri::localname()
{

  return localname_;
}


string&	http::uri::widename()
{
  return widename_;
}


const string&	http::uri::extension()
{
	return (extension_);
}

bool	http::uri::build_extension()
{
	char	*str;
	char	*tmp;

	str = (char *)localname_.c_str();
	tmp = (char *)localname_.c_str() + localname_.size() - 1;
	while (*tmp != '/')
	{
		if (*tmp == '.')
			break ;
		tmp--;
	}
	extension_ = ++tmp;
	return (false);
}

bool	http::uri::normalize()
{
	char	*str;

	str = (char *)localname_.c_str();
	while (*str)
	{
		if (*str == '/')
			*str = sysapi::file::delim;
		str++;
	}
	return (true);
}