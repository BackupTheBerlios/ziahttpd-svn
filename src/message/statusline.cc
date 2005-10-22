#include <vector>
#include <iostream>
#include <message.hh>
#include <stringmanager.hh>
#include <httpstringmanager.hh>

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
	//page_is_dir = false;
	//if (page_[page_.size() - 1] == '/')
	//	page_is_dir = true;
	//path_ = "c:/www" + page_;
	return (true);
}

