//
// modimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 09:53:34 2006 texane
// Last update Thu Mar 23 09:59:16 2006 texane
//


#include "include/mod_encoding.hh"


using namespace std;


// implement IModule

mod_encoding::mod_encoding()
{
}

mod_encoding::~mod_encoding()
{
}

bool mod_encoding::ReadConfig(const char* nm_conf)
{
  return false;
}

const char* mod_encoding::GetModuleName()
{
  return "mod_encoding";
}

const char* mod_encoding::GetModuleVersion()
{
  return "1.0";
}

void mod_encoding::OnLoad(IFS*)
{
}
