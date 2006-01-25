#include <core/ziafs_net.hh>
#include <core/zia_stringmanager.hh>
#include <core/ziafs_debug.hh>

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

status::error	net::http::consume(net::session *s, buffer &buf)
{
	std::string	ln;
	if (m_line.from_buffer(ln, buf) == true)
	{
		ziafs_debug_msg("line : %s\n", ln.c_str());
	}
	//if (state != BODY)
	//{
	//	// if readline
	//		// parsing
	//		// 

	//} else {
	//	// readdata
	//}

	ziafs_return_status(status::NOTIMPL);
}

status::error	net::http::produce(buffer &buf)
{
	ziafs_return_status(status::NOTIMPL);
}
