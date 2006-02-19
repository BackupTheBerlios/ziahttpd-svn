#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>

std::string&			net::http::http_data::operator[](const std::string& key)
{
	std::string str(key);

	//stringmanager::normalize(str);
	return (m_hdrlines[str]);
}

std::string&			net::http::http_data::operator=(const std::string& val)
{
	return ((std::string&)val);
}
