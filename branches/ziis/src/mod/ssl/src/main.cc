//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 13:31:04 2006 texane
// Last update Sat Apr 01 10:30:01 2006 texane
//


#include "include/modimpl.hh"


using namespace std;


// instanciate the module

IModule* GetNewInstance()
{
  cout << "[modssl] GetNewInstance()" << endl;
  return new mod_ssl;
}


// implement IModule

bool mod_ssl::ReadConfig(const char* nm_conf)
{
  return false;
}

const char* mod_ssl::GetModuleName()
{
  return "mod_ssl";
}

const char* mod_ssl::GetModuleVersion()
{
  return "1.0";
}

void mod_ssl::OnLoad(IFS*)
{
}



// implement IConnection

short mod_ssl::GetPort()
{
  return m_port;
}

const char* mod_ssl::GetHost()
{
  return m_host.c_str();
}

void* mod_ssl::Accept(SOCKET)
{
  cout << "[module] module->Accept()" << endl;
  return 0;
}

int mod_ssl::Recv(SOCKET h_sock, void* p_data, char* p_buf, int ln_buf)
{
  sysapi::error::handle_t h_err;
  unsigned int nr_recv;

  cout << "[module] module->Recv()" << endl;
  h_err = sysapi::insock::recv(h_sock, (unsigned char*)p_buf, ln_buf, nr_recv);
  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return (int)nr_recv;
}

int mod_ssl::Send(SOCKET h_sock, void* p_data, const char* p_buf, int ln_buf)
{
  sysapi::error::handle_t h_err;
  unsigned int nr_sent;

  cout << "[module] module->Send()" << endl;
  h_err = sysapi::insock::send(h_sock, (unsigned char*)p_buf, ln_buf, nr_sent);
  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return nr_sent;
}

void mod_ssl::Close(SOCKET, void*)
{
  cout << "[module] module->Close()" << endl;
}



// internal management

void mod_ssl::reset()
{
  m_port = 0;
  m_host.clear();
}

void mod_ssl::release()
{
}

bool mod_ssl::reload(const string& nm_conf)
{
  release();
  reset();
  return true;
}

mod_ssl::mod_ssl(const string& nm_conf)
{
  reload(nm_conf);
}

mod_ssl::~mod_ssl()
{
}
