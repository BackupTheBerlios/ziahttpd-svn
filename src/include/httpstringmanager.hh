/********************************************************************
	created:	2005/10/20
	created:	20:10:2005   8:34
	filename: 	e:\work\c++\zia-httpd\src\include\httpstringmanager.hh
	file path:	e:\work\c++\zia-httpd\src\include
	file base:	httpstringmanager
	file ext:	hh
	author:		
	
	purpose:	
*********************************************************************/

#include <string>
#include <map>

namespace	stringmanager
{
	class	httpsm
	{
	public:
		bool	method(const std::string&, std::string&, int&);
		bool	url(const std::string&, std::string&, int&);
		bool	query(const std::string&, std::map<std::string, std::string> &, int&);
		bool	version(const std::string&, std::string&, int&);
	//	bool	get_var(const std::string&, std::map<std::string, std::string> &, int&);
	protected:
	private:
	};
};
