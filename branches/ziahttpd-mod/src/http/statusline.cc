#include <vector>
#include <iostream>
#include <message.hh>
#include <stringmanager.hh>
#include <httpstringmanager.hh>
#include <session.hh>

bool		http::message::uri(const std::string &str, int& err)
{
	std::string					tmpp;
	std::string					tmpq;
	stringmanager::httpsm		hp;

#ifdef _DEBUG
	std::cout << "function uri" << std::endl;
#endif
	std::string::size_type pos = str.find ("?",0);
	if (pos == std::string::npos)
	{
		tmpp = str;
		tmpq = "";
	} else {
		tmpp = str.substr(0, pos);
		tmpq =  str.substr(pos + 1, str.size());
#ifdef _DEBUG
		std::cout << "[" << tmpp << "][" << tmpq << "]" << std::endl;
#endif
		if (!hp.query(tmpq, getquery_, error_code_))
			return (false);
	}
	if (!hp.url(tmpp, page_, error_code_))
		return (false);
	return (true);
}

