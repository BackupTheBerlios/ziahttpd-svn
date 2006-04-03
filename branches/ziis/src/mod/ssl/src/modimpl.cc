//
// modimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 01 11:13:09 2006 texane
// Last update Mon Apr  3 18:43:26 2006 
//


#include "include/modimpl.hh"


using namespace std;


// impleement the IModule inteface



// instanciate the module

IModule* GetNewInstance()
{
  cout << "new instance of ssl" << endl;
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
  // initialize the sslmodule, global wide setting
}
