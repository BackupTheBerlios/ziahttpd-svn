#include <message.hh>
#include <session.hh>

bool		http::header::response_header_content_length(server::session *session_,																  const std::string& str)
{
//	session_->http_info_.is_body_ = true;
//	session_->http_info_.sz_body_ = (sysapi::socket_in::size_t)atoi(str.c_str());
	return (true);
}
