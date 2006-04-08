//
// conimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 08 14:31:06 2006 texane
// Last update Sat Apr 08 16:44:02 2006 texane
//


#include "include/mod_tmsock.hh"


using namespace std;


// implement IConnection

short mod_tmsock::GetPort()
{
  return m_port;
}

const char* mod_tmsock::GetHost()
{
  return m_host.c_str();
}

void* mod_tmsock::Accept(SOCKET)
{
  return 0;
}

int mod_tmsock::Recv(SOCKET h_sock, void* p_data, char* p_buf, int ln_buf)
{
  sysapi::error::handle_t h_err;
  unsigned int nr_recv;

  enter(h_sock, p_data);
  h_err = sysapi::insock::recv(h_sock, (unsigned char*)p_buf, ln_buf, nr_recv);
  leave();

  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return (int)nr_recv;
}

int mod_tmsock::Send(SOCKET h_sock, void* p_data, const char* p_buf, int ln_buf)
{
  sysapi::error::handle_t h_err;
  unsigned int nr_sent;

  h_err = sysapi::insock::send(h_sock, (unsigned char*)p_buf, ln_buf, nr_sent);
  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return nr_sent;
}

void mod_tmsock::Close(SOCKET id_socket, void* p_data)
{
  try
    {
      sysapi::insock::close(id_socket);
    }
  catch(...)
    {
      cout << "bad close handled" << endl;
    }
}



// internal management

void mod_tmsock::reset()
{
  m_port = 0;
  m_host.clear();
}

void mod_tmsock::release()
{
}

bool mod_tmsock::reload(const string& nm_conf)
{
  release();
  reset();
  return true;
}
