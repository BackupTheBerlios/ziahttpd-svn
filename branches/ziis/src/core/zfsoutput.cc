#include <ziis_impl.hh>
#include <ziafs_http.hh>

ZfsOutput::ZfsOutput(net::http& proto)
{
	m_proto = &proto;
}

ZfsOutput::ZfsOutput(thr::pool::session_t& s)
{
	m_proto = &s.proto;
	m_session = &s;
}

void	ZfsOutput::SetOutput(const char*key, const char*value)
{
	m_proto->response[key] = value;
}

const	char*	ZfsOutput::GetOutput(const char*key)
{
	std::string	val = m_proto->response[key];

	if (val.empty())
		return NULL;
	return val.c_str();
}

void	ZfsOutput::SetStatusCode(int st)
{
	m_proto->get_uri().status_code() = st;
}

bool	ZfsOutput::SendHeader()
{
	buffer	header;

	m_proto->create_header(header, 20, net::http::CHUNK_FIRST);

	return true;
}