//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 11:02:05 2006 texane
// Last update Tue Mar 21 13:07:17 2006 texane
//


#include <sys/sysapi.hh>
#include <ziis.hh>



class mod_default : public IModule, public IConnection
{
public:
  // implement IModule
  bool ReadConfig(const char*) { return false; }
  const char* GetModuleName() { return 0; }
  const char* GetModuleVersion() { return 0; }
  void OnLoad(IFS*) {}

  // implement IConnection
  short GetPort() { return 42; }
  const char* GetHost() { return 0; }
  void* Accept(SOCKET) { return 0; }
  int Recv(SOCKET, void*, char*, int) { return -1; }
  int Send(SOCKET, void*, const char*, int) { return -1; }
  void Close(SOCKET, void*) {}
};


IModule* GetNewInstance()
{
  return 0;
}
