//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 13:31:04 2006 texane
// Last update Sat Apr 01 15:01:40 2006 texane
//


#include <iostream>
#include <string>
#include <sys/sysapi.hh>
#include <ziis.hh>


using namespace std;


class mod_sock : public IModule, public IConnection
{
public:
  // ctor/dtor
  mod_sock(const string& = "conf/ziafs.xml");
  ~mod_sock();

  // implement IModule
  bool ReadConfig(const char*);
  const char* GetModuleName();
  const char* GetModuleVersion();
  void OnLoad(IFS*);

  // implement IConnection
  short GetPort();
  const char* GetHost();
  void* Accept(SOCKET);
  int Recv(SOCKET, void*, char*, int);
  int Send(SOCKET, void*, const char*, int);
  void Close(SOCKET, void*);


private:
  string m_host;
  short m_port;

  // module are resetable
  void reset();
  void release();
  bool reload(const string&);
};



// instanciate the module

IModule* GetNewInstance()
{
  cout << "[module] GetNewInstance()" << endl;
  return new mod_sock;
}


// implement IModule

bool mod_sock::ReadConfig(const char* nm_conf)
{
  return false;
}

const char* mod_sock::GetModuleName()
{
  return "mod_sock";
}

const char* mod_sock::GetModuleVersion()
{
  return "1.0";
}

void mod_sock::OnLoad(IFS*)
{
}



// implement IConnection

short mod_sock::GetPort()
{
  return m_port;
}

const char* mod_sock::GetHost()
{
  return m_host.c_str();
}

void* mod_sock::Accept(SOCKET)
{
  cout << "[module] module->Accept()" << endl;
  return 0;
}

int mod_sock::Recv(SOCKET h_sock, void* p_data, char* p_buf, int ln_buf)
{
  sysapi::error::handle_t h_err;
  unsigned int nr_recv;

  cout << "[module] module->Recv()" << endl;
  h_err = sysapi::insock::recv(h_sock, (unsigned char*)p_buf, ln_buf, nr_recv);
  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return (int)nr_recv;
}

int mod_sock::Send(SOCKET h_sock, void* p_data, const char* p_buf, int ln_buf)
{
  sysapi::error::handle_t h_err;
  unsigned int nr_sent;

  cout << "[module] module->Send()" << endl;
  h_err = sysapi::insock::send(h_sock, (unsigned char*)p_buf, ln_buf, nr_sent);
  if (h_err != sysapi::error::SUCCESS)
    return -1;
  return nr_sent;
}

void mod_sock::Close(SOCKET id_socket, void*)
{
  cout << "[module] module->Close()" << endl;
  sysapi::insock::close(id_socket);
}



// internal management

void mod_sock::reset()
{
  m_port = 0;
  m_host.clear();
}

void mod_sock::release()
{
}

bool mod_sock::reload(const string& nm_conf)
{
  release();
  reset();
  return true;
}

mod_sock::mod_sock(const string& nm_conf)
{
  reload(nm_conf);
}

mod_sock::~mod_sock()
{
}
