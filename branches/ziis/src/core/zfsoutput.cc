//
// zfsoutput.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 21:45:33 2006 texane
// Last update Thu Apr 06 18:35:33 2006 texane
//


#include <list>
#include <sys/sysapi.hh>
#include <ziis.hh>
#include <ziis_impl.hh>
#include <ziafs_http.hh>
#include <iostream>
#include <sstream>


// #define DEBUG_STEP

using namespace std;

#ifndef _WIN32
extern int stricmp(const char*, const char*);
#endif // ! _WIN32

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
	if (!value)
	{
		m_proto->response.m_hdrlines.erase(key);
		return ;
	}
	if (*value)
		m_proto->response[key] = value;
	else
		m_proto->response.m_hdrlines.erase(key);

}

const	char*	ZfsOutput::GetOutput(const char*key)
{
	if (m_proto->response[key].empty())
		return NULL;
	return m_proto->response[key].c_str();
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
      // finally send the buffer
      nr_sent = m_session->m_conn_module->Send(m_session->cli_sock, m_session->m_conn_data, ptr, ln_buf);
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
  string str_expect;
  buffer buf_expected;
  const char* p_buf;
  int ln_buf;

  str_expect = m_proto->request["expect"];
  if (str_expect.empty() == false)
    {
      size_t i = m_proto->request["expect"].find("-", 0);
      string st_code;
      if (i != std::string::npos)
	st_code = m_proto->request["expect"].substr(0, i);
      buf_expected = m_proto->request.m_version + " " + st_code + "\r\n\r\n";
    }

  m_proto->stringify_header(header);
  header = buf_expected + header;

  p_buf = header.c_str();
  ln_buf = (int)strlen(p_buf);
  ln_buf = send_whole_buffer(p_buf, ln_buf);
  delete[] p_buf;

  if (ln_buf == -1)
    return false;
  return true;
}

int ZfsOutput::SendBuffer(const char* buf, int sz)
{
  const char* te;
  const char* ce;
  list<IStreamModifier*>::iterator i_curr;
  list<IStreamModifier*>::iterator i_last;
  buffer buf_header;
  buffer buf_entity((unsigned char*)buf, (unsigned int)sz);
  buffer buf_out;
  buffer buf_remain;

  // apply stream modifiers
  i_curr = m_session->m_modifiers.begin();
  i_last = m_session->m_modifiers.end();
  while (i_curr != i_last)
    {
      buf_out.Clear();
      buf_remain.Clear();
      (*i_curr)->Transform(buf_entity, buf_out, buf_remain);
      buf_entity = buf_out + buf_remain;
      ++i_curr;
    }

#ifdef DEBUG_STEP
  cout << "------- entering" << endl;
  cout << "size is " << sz << endl;
  cout << "ensz is " << (unsigned int)buf_entity.size() << endl;
  cout << buf_entity.tostring() << endl;
  getchar();
#endif // DEBUG_STEP


  // apply compressor
  ce = GetOutput("content-encoding");
  if (ce && m_session->m_comp_out_module)
    {
      if (m_session->m_comp_out_module->Compress(m_session->m_comp_out_context, buf_entity, buf_out))
	{
	  buf_entity = buf_out;
	  sz = (int)buf_entity.size();
	}
    }

  // chunk header generation
  te = GetOutput("transfer-encoding");
  if (te && !stricmp(te, "chunked"))
    {
      net::generate_chunk_header(buf_header, sz, net::CHUNK_FIRST);
      buf_entity = buf_header + buf_entity;
      buf_entity += "\r\n";
    }
  
#ifdef DEBUG_STEP
  cout << "-------- sending" << endl;
  cout << buf_entity.tostring() << endl;
  getchar();
#endif // DEBUG_STEP

  // send the buffer
  return send_whole_buffer((const char*)buf_entity.bufptr(), (int)buf_entity.size());
}

#define ERROR_PAGE "<html><body> Request Error</body></html>"
int ZfsOutput::SendError(int err_no)
{
  ostringstream oss;
  buffer buf_error((unsigned char*)ERROR_PAGE, sizeof(ERROR_PAGE) - 1);
  char* p_buf;

  SetStatusCode(err_no);
  oss << sizeof(ERROR_PAGE) - 1;
  SetOutput("content-length", oss.str().c_str());
  SendHeader();
  p_buf = (char*)buf_error.c_str();
  SendBuffer(p_buf, buf_error.size());
  delete[] p_buf;
  return buf_error.size();
}
