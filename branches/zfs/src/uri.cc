#include <ziafs_http.hh>

status::error	net::uri::extension(std::string& localname)
{
	ziafs_return_status(status::NOTIMPL);
}

bool				net::uri::reset()
{
	m_wwwname.clear();
	m_localname.clear();
	m_status_code = 0;
	return true;
}
