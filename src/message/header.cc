#include <message.hh>
#include <session.hh>
#include <iostream>

bool		http::message::request_header_content_length(server::session *session, const std::string& str)
{
//	session->http_info_.is_body_ = true;
//	session->http_info_.sz_body_ = (sysapi::socket_in::size_t)atoi(str.c_str());
	session_->http_info_.request_body_.size((size_t)atoi(str.c_str()));
	return (true);
}
