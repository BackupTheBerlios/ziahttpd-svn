//
// modimpl.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 01 10:28:21 2006 texane
// Last update Sat Apr 01 10:59:50 2006 texane
//


#ifndef MODIMPL_HH
# define MODIMPL_HH


#include "openssl/ssl.h"
#include <iostream>
#include <string>
#include <sys/sysapi.hh>
#include <ziis.hh>


class mod_ssl : public IModule, public IConnection
{
public:
  // ctor/dtor
  mod_ssl(const std::string& = "conf/ziafs.xml");
  ~mod_ssl();

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
  std::string m_host;
  short m_port;

  // module are resetable
  void reset();
  void release();
  bool reload(const std::string&);
};


#endif //MODIMPL_HH
