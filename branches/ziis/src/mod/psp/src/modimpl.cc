//
// modimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 16:38:48 2006 texane
// Last update Thu Mar 23 13:59:38 2006 texane
//


#include "include/mod_psp.hh"


// implement IModule

char *m_mimes[] = {
									"text/psp",
									0};

bool mod_psp::ReadConfig(const char* path)
{
  //if (conf)
  //  {
  //    delete conf;
  //    conf = NULL;
  //  }

  //conf = new net::config(path);
  return true;
}

const char* mod_psp::GetModuleName()
{
  return "mod_psp";
}

const char* mod_psp::GetModuleVersion()
{
  return "1.0";
}

void mod_psp::OnLoad(IFS*)
{

}

mod_psp::mod_psp()
{
	mimes = (char **)m_mimes;
}