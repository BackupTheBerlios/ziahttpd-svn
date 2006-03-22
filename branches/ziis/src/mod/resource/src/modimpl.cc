//
// modimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 16:38:48 2006 texane
// Last update Wed Mar 22 16:42:21 2006 texane
//


#include "include/mod_resource.hh"


// implement IModule

bool mod_resource::ReadConfig(const char* path)
{
	if (conf)
	{
		delete conf;
		conf = NULL;
	}

	conf = new net::config(path);
  return true;
}

const char* mod_resource::GetModuleName()
{
  return "mod_resource";
}

const char* mod_resource::GetModuleVersion()
{
  return "1.0";
}

void mod_resource::OnLoad(IFS*)
{
}
