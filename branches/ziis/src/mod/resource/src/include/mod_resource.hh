//
// mod_resource.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 16:40:22 2006 texane
// Last update Wed Mar 22 16:41:49 2006 texane
//


#ifndef MOD_RESOURCE_HH
# define MOD_RESOURCE_HH


#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/sysapi.hh>
#include <ziis.hh>
#include "include/buffer.hh"
#include "include/resource.hh"
#include "include/constants.hh"


class mod_resource : public IModule, public IDocumentGenerator
{
public:
  mod_resource();
  ~mod_resource();

  // implement IModule
  bool ReadConfig(const char*);
  const char* GetModuleName();
  const char* GetModuleVersion();
  void OnLoad(IFS*);

  // implement IDocumentGenerator
  void GenerateDocument(IInput&, const char*, IOutput&);
  const char** GetSupportedMime();

};


#endif // ! MOD_RESOURCE_HH
