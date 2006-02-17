#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>

bool			net::http::generate_status_line()
{
	std::string	ln;
	std::string	st_code;

	ln = response.m_version + " " + "200" + " ";
	error_code_string(st_code);
	ln += st_code;
	return true;
}