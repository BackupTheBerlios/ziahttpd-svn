//
// zfsoutput.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 21:45:33 2006 texane
// Last update Wed Mar 22 22:03:25 2006 texane
//


#include <list>
#include <sys/sysapi.hh>
#include <ziis.hh>
#include <ziis_impl.hh>
#include <ziafs_http.hh>


using namespace std;


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

ZfsOutput::ZfsOutput(thr::pool::session_t* s)
{
	m_proto = &s->proto;
	m_session = s;
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


int ZfsOutput::send_whole_buffer(const char* p_buf, int ln_buf, bool do_filtering)
{
  bool is_error;
  const char* ptr;
  int nr_sent;

  is_error = false;
  ptr = p_buf;
  while (ln_buf > 0 && is_error == false)
    {
      // pass thru all filters
      if (do_filtering == true)
	{
	  // pass thru stream modifiers
	  list<IStreamModifier*>::iterator i_curr;
	  list<IStreamModifier*>::iterator i_last;

	  i_curr = m_session->m_modifiers.begin();
	  i_last = m_session->m_modifiers.end();
	  while (i_curr != i_last)
	    {
	      // (*i_curr)->Transform();
	      ++i_curr;
	    }

	  // pass thru compressor
	  if (m_session->m_comp_module)
	    {
	      // m_session->m_comp_module->Decompress(m_session->m_comp_data, , );
	    }
	}

      // finally send the buffer
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
  return send_whole_buffer(buf, sz, true);
}

int ZfsOutput::SendError(int)
{
#define ERROR_PAGE "<html><body> ERROR PAGE </body></html>"
  return send_whole_buffer(ERROR_PAGE, sizeof(ERROR_PAGE) - 1);
}
