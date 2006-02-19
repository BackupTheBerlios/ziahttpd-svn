#include <ziafs_http.hh>

status::error	net::uri::extension(std::string& localname)
{
	size_t i = m_wwwname.rfind(".", m_wwwname.length() - 1);
	if (i != std::string::npos)
	{
		localname = m_wwwname.substr(i + 1, m_wwwname.length() - i - 1);
	}
	ziafs_return_status(status::NOTIMPL);
}

bool				net::uri::reset()
{
	m_wwwname.clear();
	m_localname.clear();
	m_status_code = 0;
	return true;
}

net::uri::uri()
{
	reset();
}
