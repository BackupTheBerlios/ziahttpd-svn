//
// mod_resource.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 16:40:22 2006 texane
// Last update Mon Apr 10 01:50:47 2006 
//


#ifndef MOD_RESOURCE_HH
# define MOD_RESOURCE_HH


#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <sys/sysapi.hh>
#include <ziis.hh>
#include "buffer.hh"
#include "utils.hh"

#ifdef _WIN32
# include "include/resource.hh"
# include "include/constants.hh"
# include "include/config.hh"
#else
# include "resource.hh"
# include "constants.hh"
# include "config.hh"
#endif // _WIN32


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
private:
  net::config *conf;
};


#endif // ! MOD_RESOURCE_HH
