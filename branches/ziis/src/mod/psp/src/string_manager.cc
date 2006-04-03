#include "include/string_manager.hh"
#include <iostream>

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

bool	stringmanager::uppercase(std::string &str)
{
	for (size_t i = 0 ; i < str.size(); i++)
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] - 'a' + 'A';
	return (true);
}

bool	stringmanager::normalize(char *str)
{
	for (size_t i = 0 ; i < strlen(str); i++)
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] - 'A' + 'a';
	return (true);
}

bool	stringmanager::unconvert_hexa(const std::string &source, std::string &dest)
{
	dest = source;
	return (unconvert_hexa(dest));
}

bool	stringmanager::convert_hexa(const std::string &source, std::string &dest)
{
	return (true);
}
bool	stringmanager::convert_hexa(std::string &str)
{
	return (true);
}
bool	stringmanager::unconvert_hexa(std::string &str)
{
	for (int i = 0; i < str[i]; i++)
	{
		if (str[i] == '%')
		{
			std::string	strr;
			char	c[2];

			strr = str.substr(i + 1, 2);
			if (!hex_to_char(strr, c[0]))
				return (false);
			c[1] = '\0';
			str.replace(i, 1, c);
			str.erase(i+1, 2);
		}
	}
	return (true);
}

bool	stringmanager::hex_to_char(const std::string &source, char &dest)
{
	int		len = (int)source.size();
	char	alf[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	int		res = 0;
	int		loop_ = 1;
	int		flag;
	char	cur;

	for (int i = (len - 1); i != -1; i--)
	{
		cur = source[i];
		if (cur >= 'a' && cur <= 'f')
			cur = cur - 'a' + 'A';
		flag = 0;
		for (int t = 0; alf[t]; t++)
		{
			if (alf[t] == cur)
			{
				flag = 1;
				res += t * loop_;
				break ;
			}
		}
		if (!flag)
			return (false);
		loop_ *= 16;
	}
	dest = res;
	return (true);
}

bool	stringmanager::dec_to_hex(const int nb, std::string& dest)
{
	char	tmp[10];

	sprintf(tmp, "%x", nb);
	dest = tmp;
	return (true);
}
bool	stringmanager::remove_space(std::string& str)
{
	while (str[0] == ' ')
		str.erase(0, 1);
	return (true);
}

bool stringmanager::hex_to_int(const std::string& hexnumber, int& number)
{ 
	char* tmp; 
	number = strtol(hexnumber.c_str(), &tmp, 16);
	return (true);
}
