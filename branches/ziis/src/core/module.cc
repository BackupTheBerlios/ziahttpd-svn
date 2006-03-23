//
// module.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 15:17:50 2006 texane
// Last update Thu Mar 23 16:50:46 2006 texane
//


// implement module management


#include <list>
#include <queue>
#include <string>
#include <string.h>
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
	{
	  return true;
	}
      ++it_curr;
    }

  p_mod = 0;
  return false;
}

bool mod::manager::get_compressor_module(ICompressor*& p_mod, const string& accepted_encoding, string& chosen_encoding)
{
  // match the supported encoding
  // match the mime type
  list<modinfo*>::iterator it_curr;
  list<modinfo*>::iterator it_last;
  string curr_encoding;
  const char** arr_encodings;
  unsigned int i_encoding;

  curr_encoding = "deflate";
  it_curr = m_modlist.begin();
  it_last = m_modlist.end();
  while (it_curr != it_last)
    {
      if ((p_mod = dynamic_cast<ICompressor*>((*it_curr)->m_instance)))
	{
	  arr_encodings = p_mod->GetSupportedEncoding();
	  for (i_encoding = 0; arr_encodings && arr_encodings[i_encoding]; ++i_encoding)
	    {
	      if (!strcmp(arr_encodings[i_encoding], curr_encoding.c_str()))
		{
		  chosen_encoding = arr_encodings[i_encoding];
		  return true;
		}
	    }
	}
      ++it_curr;
    }
  p_mod = 0;
  return false;
}

bool mod::manager::get_generator_module(IDocumentGenerator*& p_mod, const string& mimetype)
{
  // match the mime type
  list<modinfo*>::iterator it_curr;
  list<modinfo*>::iterator it_last;
  const char** arr_mime;
  unsigned int i_mime;

  it_curr = m_modlist.begin();
  it_last = m_modlist.end();
  while (it_curr != it_last)
    {
      if ((p_mod = dynamic_cast<IDocumentGenerator*>((*it_curr)->m_instance)))
	{
	  arr_mime = p_mod->GetSupportedMime();
	  for (i_mime = 0; arr_mime && arr_mime[i_mime]; ++i_mime)
	    {
// 	      if (!strcmp(arr_mime[i_mime], mimetype.c_str()))
		{
		  return true;
		}
	    }
	}
      ++it_curr;
    }

  p_mod = 0;
  return false;
}


bool mod::manager::get_modifier_list(list<IStreamModifier*>& lst_modifiers, list<string>& lst_mimes)
{
  // match the content type + order by priority

  list<modinfo*>::iterator i_curr;
  list<modinfo*>::iterator i_last;
  list<string>::iterator i_mime;
  list<IStreamModifier*>::iterator i_node;
  IStreamModifier* p_mod;
  const char** arr_type;
  unsigned int i_type;

  i_curr = m_modlist.begin();
  i_last = m_modlist.end();
  while (i_curr != i_last)
    {
      if ((p_mod = dynamic_cast<IStreamModifier*>((*i_curr)->m_instance)))
	{
	  arr_type = p_mod->GetSupportedContentType();
	  for (i_type = 0; arr_type && arr_type[i_type]; ++i_type)
	    {
	      for (i_mime = lst_mimes.begin(); i_mime != lst_mimes.end(); ++i_mime)
		{
		  if (!strcmp(arr_type[i_type], i_mime->c_str()))
		    {
		      for (i_node = lst_modifiers.begin(); i_node != lst_modifiers.end() && (*i_node)->GetPriority() < p_mod->GetPriority(); ++i_node)
			;
		      lst_modifiers.insert(i_node, p_mod);
		    }
		}
	    }
	}
      ++i_curr;
    }
  return true;
}

bool mod::manager::load_module(const string& path, const string& conf)
{
  modinfo* p_info;

  p_info = new modinfo(path);
  if (p_info->m_instance)
    {
      p_info->m_instance->OnLoad(0);
      p_info->m_instance->ReadConfig(conf.c_str());
    }
  m_modlist.push_front(p_info);
  return true;
}

bool mod::manager::reload(net::config* p_config)
{
  list<net::config::module*>::iterator curr_mod;

  p_config->get_modules(curr_mod);
  while (p_config->end_modules(curr_mod) == false)
    {
      load_module((*curr_mod)->file, "conf/ziafs.xml");
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
