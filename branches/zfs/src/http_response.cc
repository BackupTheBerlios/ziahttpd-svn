#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>

bool			net::http::generate_status_line()
{
	std::string	ln;
	std::string	st_code;

	ln = m_version + " " + m_uri.status_code() + " ";
	error_code_string(st_code);
	ln += st_code;

}