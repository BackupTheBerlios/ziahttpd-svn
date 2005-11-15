//
// modman.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:34:44 2005 
// Last update Mon Nov 14 13:20:09 2005 
//


#include <list>
#include <string>
#include <server/modman.hh>
#include <server/module.hh>
#include <http/session.hh>
#include <sysapi/sysapi.hh>


using std::list;
using std::string;


// Singleton-like behaviour
server::modman* server::modman::instance_ =  0;


server::modman::modman()
{
  instance_ = this;
}


bool	server::modman::is_loaded(const std::string& name, list<module*>::iterator* pos)
{
  list<module*>::iterator cur = modlist_.begin();
  list<module*>::iterator end = modlist_.end();

  while (cur != end)
    {
      if ((*cur)->name_ == name)
	{
	  if (pos)
	    *pos = cur;
	  return true;
	}
      ++cur;
    }

  return false;
}


bool	server::modman::load_at_beginning(const string& name, bool priviledged, bool active)
{
  if (is_loaded(name) == false)
    modlist_.push_front(new module(name, priviledged, active));
  return true;
}


bool	server::modman::load_at_end(const string& name, bool priviledged, bool active)
{
  if (is_loaded(name) == false)
    modlist_.push_back(new module(name, priviledged, active));
  return true;
}


bool	server::modman::load(const string& after_name,
			     const string& my_name,
			     const string& before_name,
			     bool priviledged,
			     bool active,
			     bool load_missing)
{
  list<module*>::iterator after_pos;
  list<module*>::iterator before_pos;

  if (is_loaded(after_name, &after_pos) == false)
    {
      if (load_missing == true && load_at_beginning(after_name, priviledged, active) == false)
	return false;
      after_pos = modlist_.begin();
    }

  if (is_loaded(before_name, &before_pos) == false)
    {
      if (load_missing == true && load_at_beginning(before_name, priviledged, active) == false)
	return false;
      before_pos = modlist_.begin();
    }

  modlist_.insert(after_pos, new module(my_name, priviledged, active));

  return true;
}


bool	server::modman::reload(const string& name, bool priviledged, bool active)
{
  // Load the module if it doesn't exist.

  list<module*>::iterator pos;

  if (is_loaded(name) == false)
    load_at_beginning(name, priviledged, active);
  else
    {
      delete *pos;
      *pos = new module(name, priviledged, active);
    }

  return true;
}


bool	server::modman::unload(const string& name)
{
  list<module*>::iterator pos;

  if (is_loaded(name, &pos) == false)
    return false;

  modlist_.erase(pos);
  delete *pos;

  return true;
}


bool	server::modman::start(const string& name)
{
  list<module*>::iterator pos;

  if (is_loaded(name, &pos) == false)
    return false;

  (*pos)->running_ = true;

  return true;
}


bool	server::modman::stop(const string& name)
{
  list<module*>::iterator pos;

  if (is_loaded(name, &pos) == false)
    return false;

  (*pos)->running_ = false;

  return true;
}


bool	server::modman::state(const string&, int&)
{
  return false;
}


bool	server::modman::call_hooks(core* core, stageid_t id, http::session* session)
{
  list<module*>::iterator cur = modlist_.begin();
  list<module*>::iterator end = modlist_.end();
  bool (*module::* hook)(http::session&, server::core*, int&) = &module::hk_create_con_;
  int reason;

  // Choose the correct function to execute
  switch (id)
    {
    case CREATE_CON:
      hook = &module::hk_create_con_;
      break;
    case READ_RQST_METADATA:
      hook = &module::hk_get_rqstmetadata_;
      break;
    case READ_RQST_DATA:
      hook = &module::hk_get_rqstdata_;
      break;
    case PARSE_RQST_METADATA:
      hook = &module::hk_parse_rqstmetadata_;
      break;
    case ALTER_RQST_DATA:
      hook = &module::hk_alter_rqstdata_;
      break;
    case BUILD_RESP_METADATA:
      hook = &module::hk_build_respmetadata_;
      break;
    case BUILD_RESP_DATA:
      hook = &module::hk_build_respdata_;
      break;
    case ALTER_RESP_DATA:
      hook = &module::hk_alter_respdata_;
      break;
    case ALTER_RESP_METADATA:
      hook = &module::hk_alter_respmetadata_;
      break;
    case SEND_RESP:
      hook = &module::hk_send_response_;
      break;
    case RELEASE_CON:
      hook = &module::hk_release_con_;
      break;
    }

  // Call the hook for stage id
  while (cur != end)
    {
      if ((*cur)->*hook)
	((*cur)->*hook)(*session, (*cur)->priviledged_ ? core : 0, reason);
      ++cur;
    }

  return true;
}


server::module* server::modman::operator[](const string& name)
{
  list<module*>::iterator pos;

  if (is_loaded(name, &pos) == true)
    return *pos;

  return 0;
}


server::modman* server::modman::instance()
{
  return instance_;
}
