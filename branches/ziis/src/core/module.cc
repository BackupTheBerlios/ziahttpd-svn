//
// module.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 15:17:50 2006 texane
// Last update Tue Mar 21 16:39:35 2006 texane
//


// implement module management


#include <list>
#include <string>
#include <ziafs.hh>


using namespace std;


// pipeline implementation

mod::pipeline::pipeline()
{
  reset();
}

mod::pipeline::~pipeline()
{
  release();
}

bool mod::pipeline::rebuild(net::config* config)
{
  list<net::config::module*>::iterator curr_mod;

  release();
  reset();

  config->get_modules(curr_mod);
  while (config->end_modules(curr_mod) == false)
    {
      load_module((*curr_mod)->file);
      ++curr_mod;
    }
  return true;
}

void mod::pipeline::release()
{
  list<modinfo*>::iterator i_curr;
  list<modinfo*>::iterator i_last;

  i_curr = m_modules.begin();
  i_last = m_modules.end();
  while (i_curr != i_last)
    {
      delete *i_curr;
      ++i_curr;
    }
  m_modules.clear();
}

void mod::pipeline::reset()
{
}

bool mod::pipeline::load_module(const string& nm_module)
{
  // take into account the
  // module priority, if any

  modinfo* mod;

  mod = new modinfo(nm_module);
  m_modules.push_front(mod);
  return true;
}

bool mod::pipeline::unload_module(const string& nm_module)
{
  return false;
}

bool mod::pipeline::execute()
{
  return false;
}

bool mod::pipeline::find(const string& nm_module)
{
  return false;
}


// module implementation

mod::modinfo::modinfo(const string& nm_module)
{
  IModule* (*fptr)();

  m_id_module = 0;
  m_nm_module = nm_module;
  if (sysapi::module::load(m_h_module, nm_module) == sysapi::error::SUCCESS)
    {
      if (sysapi::module::resolve((void*&)fptr, m_h_module, "GetNewInstance") == sysapi::error::SUCCESS)
	{
	  m_id_module = fptr();
	}
      else
	{
	  cout << "resolv failed" << endl;
	  sysapi::module::unload(m_h_module);
	}
    }
}

mod::modinfo::~modinfo()
{
  if (m_id_module)
    {
      delete m_id_module;
      sysapi::module::unload(m_h_module);
    }
}
