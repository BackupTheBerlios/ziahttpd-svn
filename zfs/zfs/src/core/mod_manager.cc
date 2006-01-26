//
// mod_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Jan 26 00:23:58 2006 texane
// Last update Thu Jan 26 01:28:17 2006 texane
//


#include <list>
#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_mod.hh>
#include <core/ziafs_status.hh>


using std::list;
using std::string;


// Exported

net::mod_manager::mod_manager(config* config)
{
  // Here load the modules
}


status::error net::mod_manager::load_module(const string& name)
{
  sysapi::module::handle_t hmod;
  sysapi::error::handle_t herr;
  status::error (*mod_init_fn)(module*&);
  module* mod;

  // Load the module
  herr = sysapi::module::load(hmod, name);
  if (herr == sysapi::error::SUCCESS)
    {
      herr = sysapi::module::resolve((void*&)mod_init_fn, hmod, string("mod_init"));
      if (herr == sysapi::error::SUCCESS)
	{
	  if (mod_init_fn(mod) != status::SUCCESS)
	    ziafs_return_status( status::INIT_FAILED );
	  mod->m_name = name;
	  mod->m_hmod = hmod;
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
  ziafs_return_status( status::NOTIMPL );
}


status::error net::mod_manager::find_module(module*& mod, const string& name)
{
  ziafs_return_status( status::NOT_FOUND );  
}
