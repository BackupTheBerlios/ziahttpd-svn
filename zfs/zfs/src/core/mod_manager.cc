//
// mod_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Jan 26 00:23:58 2006 texane
// Last update Thu Jan 26 22:17:35 2006 texane
//


#include <list>
#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_mod.hh>
#include <core/ziafs_status.hh>
#include <core/ziafs_debug.hh>


using std::list;
using std::string;


// Exported

#include <iostream>
net::mod_manager::mod_manager(config* config)
{
  list<config::module*>::iterator cur;

  // Here load the modules
  config->get_modules(cur);
  while (config->end_modules(cur) == false)
    {
      load_module((*cur)->name);
      ++cur;
    }
}

net::mod_manager::~mod_manager()
{
  list<module*>::iterator cur = m_modules.begin();
  list<module*>::iterator end = m_modules.end();

  while (cur != end)
    {
      delete *cur;
      ++cur;
    }
}


status::error net::mod_manager::load_module(const string& name)
{
  sysapi::module::handle_t hmod;
  sysapi::error::handle_t herr;
  status::error (*mod_entry_fn)(module*&);
  module* mod;

  // Load the module
  herr = sysapi::module::load(hmod, name);
  if (herr == sysapi::error::SUCCESS)
    {
      herr = sysapi::module::resolve((void*&)mod_entry_fn, hmod, string("mod_entry"));
      if (herr == sysapi::error::SUCCESS)
	{
	  if (mod_entry_fn(mod) != status::SUCCESS)
	    ziafs_return_status( status::INIT_FAILED );
	  mod->m_name = name;
	  mod->m_hmod = hmod;
	  mod->accept_connection(0, "", 0);
	  m_modules.push_front(mod);
	}
      else
	{
	  sysapi::module::unload(hmod);
	  ziafs_return_status( status::NOT_FOUND );
	}
    }

  ziafs_return_status( status::SUCCESS );
}


status::error net::mod_manager::unload_module(const string& name)
{
  list<module*>::iterator cur = m_modules.begin();
  list<module*>::iterator end = m_modules.end();

  while (cur != end)
    {
      if ((*cur)->m_name == name)
	{
	  delete *cur;
	  m_modules.erase(cur);
	  ziafs_return_status( status::SUCCESS );
	}
      ++cur;
    }

  ziafs_return_status( status::NOT_FOUND );
}


status::error net::mod_manager::find_module(module*& mod, const string& name)
{
  list<module*>::iterator cur = m_modules.begin();
  list<module*>::iterator end = m_modules.end();

  while (cur != end)
    {
      if ((*cur)->m_name == name)
	{
	  mod = *cur;
	  ziafs_return_status( status::SUCCESS );
	}
      ++cur;
    }

  ziafs_return_status( status::NOT_FOUND );
}
