//
// module.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 15:17:50 2006 texane
// Last update Fri Apr  7 01:42:00 2006 
//


// implement module management


#include <list>
#include <vector>
#include <string>
#include <string.h>
#include <ziafs.hh>


using namespace std;

#ifndef _WIN32
extern int stricmp(const char*, const char*);
#endif // !_WIN32


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

  p_mod = 0;
  return false;
}

bool mod::manager::get_compressor_module(ICompressor*& p_mod, vector<string>& arr_encodings, string& chosen_encoding)
{
  // match the supported encoding
  // match the mime type
  list<modinfo*>::iterator it_curr;
  list<modinfo*>::iterator it_last;
  vector<string>::iterator it_enco;
  const char** supp_encodings;
  string curr_encoding;
  unsigned int i_encoding;

  it_curr = m_modlist.begin();
  it_last = m_modlist.end();
  while (it_curr != it_last)
    {
      if ((p_mod = dynamic_cast<ICompressor*>((*it_curr)->m_instance)))
	{
	  supp_encodings = p_mod->GetSupportedEncoding();
	  for (i_encoding = 0; supp_encodings && supp_encodings[i_encoding]; ++i_encoding)
	    {
	      for (it_enco = arr_encodings.begin(); it_enco != arr_encodings.end(); ++it_enco)
		{
		  if (!stricmp(supp_encodings[i_encoding], it_enco->c_str()))
		    {
		      chosen_encoding = *it_enco;
		      // hack me here
		      {
			chosen_encoding = "deflate";
		      }
		      return true;
		    }
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
	      if (!stricmp(arr_mime[i_mime], mimetype.c_str()))
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


bool mod::manager::get_modifier_list(list<IStreamModifier*>& lst_modifiers, const string& mimetype)
{
  // match the content type + order by priority

  list<modinfo*>::iterator i_curr;
  list<modinfo*>::iterator i_last;
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
	      if (!stricmp(arr_type[i_type], mimetype.c_str()))
		{
		  for (i_node = lst_modifiers.begin(); i_node != lst_modifiers.end() && (*i_node)->GetPriority() < p_mod->GetPriority(); ++i_node)
		    ;
		  lst_modifiers.insert(i_node, p_mod);
		}
	    }
	}
      ++i_curr;
    }
  return true;
}

bool mod::manager::load_module(const string& path, const string& conf, IFS* p_ifs)
{
  modinfo* p_info;

  p_info = new modinfo(path);
  if (p_info->m_instance)
    {
      p_info->m_instance->OnLoad(p_ifs);
      p_info->m_instance->ReadConfig(conf.c_str());
    }
  m_modlist.push_front(p_info);
  return true;
}

bool mod::manager::reload(net::config* p_config, IFS* p_ifs)
{
  list<net::config::module*>::iterator curr_mod;

  p_config->get_modules(curr_mod);
  while (p_config->end_modules(curr_mod) == false)
    {
      load_module((*curr_mod)->file, (*curr_mod)->configfile, p_ifs);
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
