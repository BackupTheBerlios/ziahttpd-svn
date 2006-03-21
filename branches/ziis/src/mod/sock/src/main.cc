//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 13:31:04 2006 texane
// Last update Tue Mar 21 13:52:31 2006 texane
//


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
  return 0;
}

int mod_sock::Recv(SOCKET, void*, char*, int)
{
  return -1;
}

int mod_sock::Send(SOCKET, void*, const char*, int)
{
  return -1;
}

void mod_sock::Close(SOCKET, void*)
{
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
