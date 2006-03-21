//
// module.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 15:17:50 2006 texane
// Last update Tue Mar 21 22:35:05 2006 texane
//


// implement module management


#include <list>
#include <string>
#include <ziafs.hh>


using namespace std;


// one manager instance by server

mod::manager::manager()
{
}

mod::manager::~manager()
{
  list<modinfo*>::iterator i_curr;
  list<modinfo*>::iterator i_last;

  i_curr = m_modlist.begin();
  i_last = m_modlist.end();
  while (i_curr != i_last)
    {
      delete *i_curr;
      ++i_curr;
    }
  m_modlist.clear();
}

bool mod::manager::get_connection_module(IConnection*& p_mod)
{
  list<modinfo*>::iterator it_curr;
  list<modinfo*>::iterator it_last;

  it_curr = m_modlist.begin();
  it_last = m_modlist.end();
  while (it_curr != it_last)
    {
      if ((p_mod = dynamic_cast<IConnection*>((*it_curr)->m_instance)))
	return true;
      ++it_curr;
    }
  return false;
}

bool mod::manager::get_compressor_module(ICompressor*& p_mod, const string& encoding)
{
  // match the supported encoding
  return false;
}

bool mod::manager::get_generator_module(IDocumentGenerator*& p_mod, const string& mimetype)
{
  // match the mime type
  return false;
}

bool mod::manager::get_modifier_list(list<IStreamModifier*>& p_list, list<string>& modifiers)
{
  // match the content type + order by priority
  return false;
}

bool mod::manager::load_module(const string& path)
{
  modinfo* p_info;

  p_info = new modinfo(path);
  return true;
}

bool mod::manager::reload(net::config* p_config)
{
  list<net::config::module*>::iterator curr_mod;

  p_config->get_modules(curr_mod);
  while (p_config->end_modules(curr_mod) == false)
    {
      load_module((*curr_mod)->file);
      ++curr_mod;
    }
  return true;
}


// module wrapper class

mod::modinfo::modinfo(const string& path)
{
  IModule* (*fptr)();

  m_instance = 0;
  m_path = path;
  if (sysapi::module::load(m_handle, path) == sysapi::error::SUCCESS)
    {
      if (sysapi::module::resolve((void*&)fptr, m_handle, "GetNewInstance") == sysapi::error::SUCCESS)
	{
	  m_instance = fptr();
	}
      else
	{
	  cout << "resolv failed" << endl;
	  sysapi::module::unload(m_handle);
	}
    }
}

mod::modinfo::~modinfo()
{
  if (m_instance)
    {
      delete m_instance;
      sysapi::module::unload(m_handle);
    }
}
