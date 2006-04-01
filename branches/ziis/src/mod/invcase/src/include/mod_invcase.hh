//
// mod_invcase.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 01 18:54:56 2006 texane
// Last update Sat Apr 01 18:57:19 2006 texane
//


#ifndef MOD_INVCASE_HH
# define MOD_INVCASE_HH


#include <iostream>
#include <sstream>
#include <string>
#include <sys/sysapi.hh>
#include <ziis.hh>


class mod_invcase : public IModule, public IStreamModifier
{
public:
  // ctor/dtor
  mod_invcase();
  ~mod_invcase();

  // implement IModule
  bool ReadConfig(const char*);
  const char* GetModuleName();
  const char* GetModuleVersion();
  void OnLoad(IFS*);

  // implement IStreamModifier
  int GetPriority();
  const char** GetSupportedContentType();
  void Transform(IBuffer&, IBuffer&, IBuffer&, bool = false);

private:
};


#endif // ! MOD_INVCASE_HH
