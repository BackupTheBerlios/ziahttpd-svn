#ifndef	STRINGMANAGER__HH
#define STRINGMANAGER__HH

#include <string>
#include <vector>

class stringmanager
{
public:
	static bool	normalize(std::string &);
	static bool	normalize(char *);
	static bool	split(const std::string&, const std::string&, std::vector<std::string>&, const std::string& putdel = "");
private:
	static bool	is_delimitor(char, const std::string &);
};

#endif