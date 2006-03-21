#include <ziis_impl.hh>
#include <ziafs_http.hh>

ZfsOutput::ZfsOutput(net::http& proto)
{
	m_proto = proto;
}
