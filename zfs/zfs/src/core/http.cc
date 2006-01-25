#include <core/ziafs_net.hh>
#include <core/zia_stringmanager.hh>

std::string&			net::http::operator[](const std::string& key)
{
	std::string str(key);

	stringmanager::normalize(str);
	return (m_hdrlines[str]);
}

std::string&			net::http::operator=(const std::string& val)
{
	std::string t = val;

	return ((std::string&)val);
}

status::error	net::http::consume(buffer &buf)
{

	ziafs_return_status(status::NOTIMPL);
}

status::error	net::http::produce(buffer &buf)
{
	ziafs_return_status(status::NOTIMPL);
}
