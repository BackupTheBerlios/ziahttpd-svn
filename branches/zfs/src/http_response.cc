#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>

bool			net::http::generate_status_line()
{
	std::string	ln;
	char				st_code[20];
	std::string	st_code_str;

	sprintf(st_code, "%i", m_uri.status_code());
	st_code_str = st_code;
	ln = response.m_version + " " + st_code_str + " ";
	error_code_string(st_code_str);
	ln += st_code_str;
	return true;
}