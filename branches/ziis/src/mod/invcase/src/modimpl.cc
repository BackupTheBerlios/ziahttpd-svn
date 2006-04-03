//
// modimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 01 11:13:09 2006 texane
// Last update Mon Apr 03 20:05:57 2006 texane
//


#include "include/mod_invcase.hh"


using namespace std;


// impleement the IModule inteface



// instanciate the module

IModule* GetNewInstance()
{
  return new mod_invcase;
}


// implement IModule

bool mod_invcase::ReadConfig(const char* nm_conf)
{
  return false;
}

const char* mod_invcase::GetModuleName()
{
  return "mod_invcase";
}

const char* mod_invcase::GetModuleVersion()
{
  return "1.0";
}

void mod_invcase::OnLoad(IFS*)
{
  // initialize the sslmodule, global wide setting
}
