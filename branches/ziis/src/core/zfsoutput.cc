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
	m_proto->create_header();
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

	m_proto->stringify_header(header);


	return true;
}

int	ZfsOutput::SendBuffer(const char*buf, int sz)
{
	return m_session->m_conn_module->Send(m_session->cli_sock, NULL, buf, sz);

}