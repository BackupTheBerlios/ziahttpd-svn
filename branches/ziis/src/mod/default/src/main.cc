//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 11:02:05 2006 texane
// Last update Tue Mar 21 13:31:50 2006 texane
//


#include <sys/sysapi.hh>
#include <ziis.hh>


class mod_default : public IModule
{
public:
  // implement IModule
  bool ReadConfig(const char*) { return false; }
  const char* GetModuleName() { return 0; }
  const char* GetModuleVersion() { return 0; }
  void OnLoad(IFS*) {}
};


IModule* GetNewInstance()
{
  return 0;
}
