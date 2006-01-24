#include <core/zia_stringmanager.hh>

bool	stringmanager::split(const std::string &source, const std::string& delimitor, std::vector<std::string>& dest, const std::string& putdel)
{
	char	*str;
	char	*tmp;
	char	*tmp2;
	std::string	cur;

	str = strdup(source.c_str());
	tmp = str;
	tmp2 = str;
	while (*tmp)
	{
		if (is_delimitor(*tmp, delimitor))
		{
			char	d;

			d = *tmp;
			*tmp = '\0';
			cur.erase();
			cur = tmp2;
			if (!cur.empty())
				dest.push_back(cur);
			if (is_delimitor(d, putdel))
			{
				cur.erase();
				cur = d;
				dest.push_back(cur);
			}
			*tmp = d;
			tmp2 = tmp + 1;
		}
		tmp++;
	}
	cur.erase();
	cur = tmp2;
	dest.push_back(cur);
	free(str);
	return (true);
}

bool	stringmanager::is_delimitor(char c, const std::string &delimitor)
{
	for (int i = 0; delimitor[i]; i++)
		if (delimitor[i] == c)
			return (true);
	return (false);
}

bool	stringmanager::normalize(std::string &str)
{
	for (size_t i = 0 ; i < str.size(); i++)
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] - 'A' + 'a';
	return (true);
}

bool	stringmanager::normalize(char *str)
{
	for (size_t i = 0 ; i < strlen(str); i++)
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] - 'A' + 'a';
	return (true);
}
