//
// modman.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:34:44 2005 
// Last update Sun Nov 27 03:09:50 2005 texane
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
  // ! this method should be recoded
  // in order to take into account
  // the reason code { NEXT_STAGE | END_STAGE | ...}

  list<module*>::iterator cur = modlist_.begin();
  list<module*>::iterator end = modlist_.end();
  bool (*module::* hook)(http::session&, server::core*, int&) = &module::hk_create_con_;
  int reason;
  bool done;

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
  // By default, let the next stage be called
  done = true;
  while (cur != end)
    {
      if ((*cur)->*hook)
	{
	  cerr << "[HOOK]: " << (*cur)->name_ << endl;
	  if (((*cur)->*hook)(*session, (*cur)->priviledged_ ? core : 0, reason) == false)
	    done = false;
	}
      ++cur;
    }

  return done;
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


bool	server::modman::next_processing_stage(http::session& session)
{
  // ? Think about connection releasing
  // ! The session is linked with this routine,
  // which is not ok... the reset_me_ should
  // be set elsewhere, and this routine should
  // only check for stageid_.

  if (session.persistent_ == false)
    {
      session.stageid_ = RELEASE_CON;
      return true;
    }

  switch (session.stageid_)
    {
    case CREATE_CON:
      session.stageid_ = READ_RQST_METADATA;
      break;

    case READ_RQST_METADATA:
      session.stageid_ = PARSE_RQST_METADATA;
      break;

    case PARSE_RQST_METADATA:
      session.stageid_ = READ_RQST_DATA;
      break;

    case READ_RQST_DATA:
      session.stageid_ = ALTER_RQST_DATA;
      break;

    case ALTER_RQST_DATA:
      session.stageid_ = BUILD_RESP_METADATA;
      break;

    case BUILD_RESP_METADATA:
      session.stageid_ = BUILD_RESP_DATA;
      break;

    case BUILD_RESP_DATA:
      session.stageid_ = ALTER_RESP_DATA;
      break;

    case ALTER_RESP_DATA:
      session.stageid_ = ALTER_RESP_METADATA;
      break;

    case ALTER_RESP_METADATA:
      session.stageid_ = SEND_RESP;
      break;

    case SEND_RESP:
      if (session.persistent_ == true)
	{
	  session.reset_me_ = true;
	  session.stageid_ = READ_RQST_METADATA;
	}
      else
	{
	  session.stageid_ = RELEASE_CON;
	}
      break;

    case RELEASE_CON:
      // Handle the special case.
      // The request connection is
      // already created, and creation
      // has to happen only once.
      session.persistent_ = false;
      break;
    }

  return true;
}
