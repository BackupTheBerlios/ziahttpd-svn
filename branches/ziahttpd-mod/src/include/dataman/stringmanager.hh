/********************************************************************
	created:	2005/10/14
	created:	14:10:2005   11:08
	filename: 	e:\work\c++\zia3\parse.hh
	file path:	e:\work\c++\zia3
	file base:	parse
	file ext:	hh
	author:		
	
	purpose:	
*********************************************************************/

#ifndef	STRINGMANAGER_PARSE_HH
#define STRINGMANAGER_PARSE_HH

#include <string>
#include <vector>

namespace	stringmanager
{
	class	string
	{
	public:
		bool	split(const std::string&, const std::string&, std::vector<std::string>&, const std::string& putdel = "");
		bool	convert_hexa(const std::string &, std::string &);
		bool	unconvert_hexa(const std::string &, std::string &);
		bool	convert_hexa(std::string &);
		bool	unconvert_hexa(std::string &);
		bool	hex_to_char(const std::string &, char &);
		bool	dec_to_hex(const int, std::string &);
		bool	normalize(std::string &);
		bool	normalize(char *);
	private:
		bool	is_delimitor(char, const std::string &);
	};
};

#endif
