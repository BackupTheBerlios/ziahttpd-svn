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


int ZfsOutput::send_whole_buffer(const char* p_buf, int ln_buf)
{
  bool is_error;
  const char* ptr;
  int nr_sent;

  is_error = false;
  ptr = p_buf;
  while (ln_buf > 0 && is_error == false)
    {
      nr_sent = m_session->m_conn_module->Send(m_session->cli_sock, NULL, ptr, ln_buf);
      if (nr_sent == -1)
	{
	  is_error = true;
	}
      else
	{
	  ln_buf -= nr_sent;
	  ptr += nr_sent;
	}
    }

  if (is_error == true)
    return false;
  return true;  
}

bool	ZfsOutput::SendHeader()
{
  buffer header;
  const char *p_buf;
  int ln_buf;

  m_proto->stringify_header(header);
  p_buf = header.c_str();
  ln_buf = (int)strlen(p_buf);
  ln_buf = send_whole_buffer(p_buf, ln_buf);
  delete[] p_buf;

  if (ln_buf == -1)
    return false;
  return true;
}

int	ZfsOutput::SendBuffer(const char* buf, int sz)
{
  return send_whole_buffer(buf, sz);
}

int ZfsOutput::SendError(int)
{
#define ERROR_PAGE "<html><body> ERROR PAGE </body></html>"
  return send_whole_buffer(ERROR_PAGE, sizeof(ERROR_PAGE) - 1);
}
