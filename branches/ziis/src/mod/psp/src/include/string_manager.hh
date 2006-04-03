#ifndef	STRINGMANAGER__HH
#define STRINGMANAGER__HH

#include <string>
#include <vector>

class stringmanager
{
public:
	static bool	normalize(std::string &);
	static bool	normalize(char *);
	static bool	uppercase(std::string&);
	static bool	split(const std::string&, const std::string&, std::vector<std::string>&, const std::string& putdel = "");
	static bool	convert_hexa(const std::string &, std::string &);
	static bool	unconvert_hexa(const std::string &, std::string &);
	static bool	convert_hexa(std::string &);
	static bool	unconvert_hexa(std::string &);
	static bool	hex_to_char(const std::string &, char &);
	static bool	dec_to_hex(const int, std::string &);
	static bool	remove_space(std::string& str);
	static bool hex_to_int(const std::string&, int&);
private:
	static bool	is_delimitor(char, const std::string &);
};

#endif
