#include <ziis_impl.hh>
#include <ziafs_http.hh>
#include <ziafs_net.hh>

ZfsInput::ZfsInput(net::http& proto)
{
	m_proto = &proto;
}

ZfsInput::ZfsInput(thr::pool::session_t& s)
{
	std::string extension;
	m_session = &s;
	m_proto = &m_session->proto;
	m_proto->get_uri().extension(extension);
	extension = "." + extension;
	s.srv->m_modman.get_generator_module(s.m_gen_module, extension);
	m_ip_client = s.cli_addr.sin_addr.S_un.S_addr;
}

ZfsInput::~ZfsInput()
{

}

const char*	ZfsInput::GetInput(const char*key)
{
	std::string	val = m_proto->request[key];

	if (val.empty())
		return NULL;
	return val.c_str();
}

const	char*	ZfsInput::GetInputMethod()
{
	return m_proto->method().c_str();
}

const char*	ZfsInput::GetInputLocation()
{
	return m_proto->get_uri().wwwname().c_str();
}

const char*	ZfsInput::GetInputHttpVersion()
{
	return m_proto->request.m_version.c_str();
}

const	char*	ZfsInput::GetInputQueryString()
{
	return m_proto->query().c_str();
}

bool	ZfsInput::GetNextHeader(char**key, char**value)
{
	static std::map<std::string, std::string>::iterator	it = m_proto->request.m_hdrlines.begin();

	if (it == m_proto->request.m_hdrlines.end())
		return false;
	*key  = (char *)(*it).first.c_str();
	*value  = (char *)(*it).second.c_str();
	it++;
	return true;
}