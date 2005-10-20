#include <httpstringmanager.hh>
#include <message.hh>
#include <iostream>
#include <vector>
#include <stringmanager.hh>

bool	stringmanager::httpsm::method(const std::string&source, std::string& dest, int& err_code)
{
	int		i;

#ifdef _DEBUG
	std::cout << "function method" << std::endl;
#endif

	for (i = 0; !http::statusline_key[i].empty(); i++)
	{
		if (http::statusline_key[i] == source)
		{
			dest = source;
			return (true);
		}
	}
	err_code = 400;
#ifdef _DEBUG
	std::cout << "function -method" << std::endl;
#endif
	return (false);
}

bool	stringmanager::httpsm::url(const std::string& source, std::string& dest, int& err_code)
{
	const char *url;
	stringmanager::string		parse;

	url = source.c_str();
	if (url[0] != '/')
	{
		err_code = 400;
		return(false);
	}
	while (url[0] == '/' && url[1] == '/')
		url++;
	dest = url;
	if (!parse.unconvert_hexa(dest))
	{
		err_code = 400;
		return (false);
	}
	return (true);
}

bool	stringmanager::httpsm::query(const std::string& source, std::map<std::string, std::string> &dest, int& err_code)
{
	std::vector<std::string>	query;
	stringmanager::string		parse;

	std::cout << "->" << source << std::endl;
	parse.split(source, "&", query);
	std::vector<std::string>::iterator theIterator;
	for( theIterator = query.begin(); theIterator != query.end(); theIterator++ )
	{
		std::vector<std::string>	q;
		parse.split(*theIterator, "=", q);
		if (q[1].empty())
			continue;
		if (q.size() == 1)
			q.push_back("");
		if (!parse.unconvert_hexa(q[0]))
		{
			err_code = 400;
			return (false);
		}
		if (!parse.unconvert_hexa(q[1]))
		{
			err_code = 400;
			return (false);
		}
		dest[q[0]] = q[1];
		#ifdef _DEBUG
		std::cout << "var [" << q[0] << "][" << q[1]<< "]" << std::endl;
		#endif
	}
	return (true);
}

bool	stringmanager::httpsm::version(const std::string& source, std::string& dest, int& err_code)
{
	std::vector<std::string>			q;
	stringmanager::string				parse;

#ifdef _DEBUG
	std::cout << "function version" << std::endl;
#endif
	parse.split(source, "/", q);
	if (q.size() != 2)
	{
		err_code = 400;
		return (false);
	}
	if (q[0] != "HTTP")
	{
		err_code = 400;
		return (false);
	}
	dest = q[1];
	return (true);
}