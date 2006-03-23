#include <iostream>
#include <ziis_impl.hh>
#include <ziafs_http.hh>
#include <ziafs_net.hh>

using namespace std;

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

ZfsInput::ZfsInput(thr::pool::session_t* s)
{
	std::string extension;
	std::string type;
	
	m_session = s;
	m_proto = &m_session->proto;
	m_proto->get_uri().extension(extension);
	genete_type_mimes(*(s->srv->srv_config), extension, type);
	if (s->srv->m_modman.get_generator_module(s->m_gen_module, type) == false)
	  {
	    cout << "[!] cannot get generator module" << endl;
	  }

	m_ip_client = s->cli_addr.sin_addr.S_un.S_addr;
}

ZfsInput::~ZfsInput()
{

}

bool				ZfsInput::genete_type_mimes(net::config& conf, std::string& ext, std::string& type)
{
	std::list<net::config::mime*>::iterator it;
	conf.get_mimes(it);

	while (conf.end_mimes(it) == false)
	{
		if ((*it)->extension == ext)
		{
			type = (*it)->type;
			return true;
		}
		++it;
	}
	type = "text/html";
	return false;
}

const char*	ZfsInput::GetInput(const char*key)
{
	if (m_proto->request[key].empty())
		return NULL;
	return m_proto->request[key].c_str();
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

int ZfsInput::GetClientIp()
{
	return m_ip_client;
}

int ZfsInput::ReadPostEntity(char* p_buf, int nr_size)
{
  int nr_read;
  int nr_total;
  bool is_done;

  is_done = false;
  nr_read = 0;
  nr_total = 0;
  while (is_done == false)
    {
      if (nr_size <= 0)
	{
	  is_done = true;
	}
      else
	{
	  nr_read = m_session->m_conn_module->Recv(m_session->cli_sock, m_session->m_conn_data, p_buf, nr_size);
	  if (nr_read < 0)
	    {
	      nr_read = -1;
	      is_done = true;
	    }
	  else
	    {
	      nr_total += nr_read;
	      nr_size -= nr_read;
	      p_buf += nr_read;
	    }
	}
    }

  // here pass thru the decompress module
//   if (m_session->m_comp_in_module)
//     {
//       IBuffer* buf_in;
//       IBuffer* buf_out;
//       buf_in = new buffer((unsigned char*)p_buf, (unsigned int)nr_size);
//       buf_out = new buffer;
//       m_session->m_comp_in_module->Decompress(buf_in, m_session->m_comp_in_data, buf_out);
//       p_buf = ...;
//       nr_total = ...;
//       delete buf_in;
//       delete buf_out;
//     }

  return nr_total;
}
