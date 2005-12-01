//
// session_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Dec 01 22:04:15 2005 texane
// Last update Thu Dec 01 22:50:57 2005 texane
//


#include <list>
#include <iostream>
#include <sysapi/sysapi.hh>
#include <server/core.hh>
#include <http/session.hh>
#include <server/modman.hh>


using std::cout;
using std::cerr;
using std::endl;
using std::list;


list<http::session*> http::session_manager::sessions_;


http::session*	http::session_manager::factory(sysapi::socket_in::handle_t hsock)
{
  session* s;
  s = new session(server::core::instance()->conf());
  s->hsock_con() = hsock;
  return s;
}


bool		http::session_manager::add(session* s)
{
  sessions_.push_front(s);
  return true;
}


bool		http::session_manager::add(sysapi::socket_in::handle_t hsock)
{
  return add(factory(hsock));
}


bool		http::session_manager::remove(session* s)
{
  sessions_.remove(s);
  delete s;
  return true;
}


bool		http::session_manager::remove(sysapi::socket_in::handle_t hsock)
{
  session* s;

  if (find_byhdl(hsock, s) == false)
    return false;

  return remove(s);
}


bool		http::session_manager::find_byhdl(sysapi::socket_in::handle_t hsock,
						  session*& sess)
{
  list<session*>::iterator cur = sessions_.begin();
  list<session*>::iterator end = sessions_.end();

  while (cur != end)
    {
      if ((*cur)->hsock_con() == hsock)
	{
	  sess = *cur;
	  return true;
	}
      ++cur;
    }

  cerr << "[! " << (unsigned int)hsock << "]: Cannot find the session" << endl;
  return false;
}


bool		http::session_manager::to_string(session*)
{ return true; }


bool		http::session_manager::to_string(sysapi::socket_in::handle_t)
{ return true; }


bool		http::session_manager::process()
{
  // ?
  // Call the hook chain for sessions
  list<session*>::iterator cur = sessions_.begin();
  list<session*>::iterator end = sessions_.end();
  http::session* session;

  while (cur != end)
    {
      session = *cur;
      session->handleio() = false;

      cout << "\t[?] Session [" << std::dec << (unsigned)session->hsock_con() << "] " << endl;

      // Make the session go through the pipeline,
      // Resetting the session if necessary
      while (session->persistent() == true &&
	     session->handleio() == false)
	{
	  if (server::core::instance()->modman_.call_hooks(server::core::instance(),
							   session->stageid_,
							   session) == true)
	    server::modman::next_processing_stage(*session);

	  if (session->reset_me_ == true)
	    {
	      session->reset();
	      cout << "\t[?] Session reseted" << endl;
	    }
	}

      // go to the next session
      ++cur;
    }

  return true;
}
