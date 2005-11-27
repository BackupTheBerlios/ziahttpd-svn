//
// core.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Oct 11 21:28:14 2005 texane
// Last update Sun Nov 27 14:43:28 2005 texane
//


#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <server/core.hh>
#include <server/server.hh>
#include <server/modhelper.hh>
#include <sysapi/sysapi.hh>
#include <dataman/conf.hh>
#include <http/session.hh>


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::list;
using http::session;


// Singleton-like behaviour
server::core* server::core::instance_ = 0;
server::service* server::core::services_ = 0;


// - Construction, destruction

server::core::core()
{
  reset();
  sysapi::socket_in::init_subsystem();
  instance_ = this;
  services_ = new service;
  ioman_ = new ioselect;
  reload_conf();
}


server::core::core(int ac, char** av)
{
  reset();
  sysapi::socket_in::init_subsystem();
  instance_ = this;
  services_ = new service;
  ioman_ = new ioselect;
  reload_conf();
}


server::core::~core()
{
  release();
  sysapi::socket_in::release_subsystem();
}


bool	server::core::reset()
{
  services_ = 0;
  instance_ = 0;
  ioman_ = 0;
  done_ = false;
  initialized_ = false;

  return true;
}


bool	server::core::release()
{
  if (ioman_)
    delete ioman_;
  return true;
}


bool	server::core::reload_conf()
{
  // Load modules
  // Currently only bootstrap with hardcoded modules
  // in the above array.

  // Load the module list
  {
  vector<string> modules;
  modules = conf_.GetListVector("modules");
  vector<string>::iterator cur = modules.begin();
  vector<string>::iterator end = modules.end();

  while (cur != end)
    {
      // Normalize the module name
      sysapi::file::normalize_name(*cur);
      if (modman_.load_at_beginning(*cur) == false)
	sysapi::error::stringify("Cannot load module");
      ++cur;
    }
  }

  return true;
}


bool	server::core::reload_conf(const string& confname)
{
  return false;
}


// ?
// This is the running function of the server, threaded
// one. keep it comment until the ioselect manager
// has been tested.

// bool	server::core::run()
// {

//   // !
//   // This is a temporary version of the network engine.
//   // One thread implement request handling, in order to begin
//   // modularization of the request processing flow.

//   for (;;)
//     {
//       // Create the server connection, accepting incoming ones
//       http::session* session = new http::session(conf_);
//       sysapi::socket_in::handle_t hsock_srv;
//       sysapi::socket_in::handle_t hsock_con;
//       sysapi::socket_in::create_listening(&hsock_srv, 40000);
//       session->hsock_srv() = hsock_srv;
//       sysapi::socket_in::accept(&hsock_con, hsock_srv);
//       session->hsock_con() = hsock_con;

//       // Create the request processing thread
//       sysapi::thread::handle_t hthread;
//       sysapi::thread::create_and_exec(&hthread, server::core::process_request, reinterpret_cast<sysapi::thread::param_t>(session));
//       sysapi::thread::wait_single(hthread);
//       sysapi::thread::release(hthread);

//       // Delete the session
//       delete session;

//     }

//   return true;
// }


bool	server::core::handle_default_connection(sysapi::socket_in::handle_t& hsock,
						dataman::buffer*,
						sysapi::socket_in::error_t&)
{
  // ?
  // Handle default connections
  // Registering a new session is matter
  // of adding new connection

  // Get the connection socket
  sysapi::socket_in::handle_t hsock_con;
  sysapi::socket_in::accept(&hsock_con, hsock);
  
  // Register ioman entry for this socket
  instance_->ioman_->register_sockhdl(hsock_con);
  instance_->register_session(hsock_con);

  // Call hook for create_connection step, that register or
  // not a hook for this one.
  cout << "[*] new socket registered" << endl;

  return true;
}


bool	server::core::run()
{
  // Here bootstrap the io manager
  // by creating a listening socket
  // on the port 40000.
  // Put a creation callback that will
  // be called, and will create session for
  // the new connections.

  // Bootstrap the server
  // Here we should have a hook or
  // module dedicated to
  // handle connection listening
  sysapi::socket_in::handle_t hsock_srv;
  sysapi::socket_in::create_listening(&hsock_srv, 40000);
  ioman_->register_sockhdl(hsock_srv);
  ioman_->read(hsock_srv, handle_default_connection, 0);

  // actually, do an accept
  for (;;)
    {
      ioman_->handle_io();
      process_sessions();
    }

  return true;
}


server::core*	server::core::instance()
{
  return instance_;
}


sysapi::thread::retcode_t	server::core::process_request(sysapi::thread::param_t param)
{
  http::session* session = reinterpret_cast<http::session*>(param);

  try
    {
    simulate_manager:
      if (session->persistent() == true)
	{
	  // Reset the session internals
	  if (session->reset_me_)
	    session->reset();

	  // Iterate over the processing stages
	  if (modman::instance()->call_hooks(core::instance(), session->stageid_, session) == true)
	    {
	      // Go to the next processing stage
	      modman::next_processing_stage(*session);
	    }
	}
      goto simulate_manager;

    }
  catch (exception::base& err)
    {
      err.report();
      err.solve();
    }
  catch (...)
    {
      cerr << "<not-a-zia> exception caugth" << endl;
    }

  return 0;
}


// Implements session management related methods

bool	server::core::register_session(const sysapi::socket_in::handle_t& hsock)
{
  http::session* session;

  session = new http::session(server::core::instance()->conf());
  session->hsock_con() = hsock;
  sessions_.push_front(session);
  return true;
}


bool	server::core::unregister_session(const sysapi::socket_in::handle_t&)
{
  return false;
}


bool	server::core::find_session_byhdl(const sysapi::socket_in::handle_t& hsock,
					 http::session*& sess)
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

  return false;
}


bool	server::core::process_sessions()
{
  // Call the hook chain
  // foreach session in the list...

  list<session*>::iterator cur = sessions_.begin();
  list<session*>::iterator end = sessions_.end();
  http::session* session;

  cout << "entering pipeline" << endl;
  while (cur != end)
    {
      session = *cur;
      session->handleio() = false;

      // Make the session go through the pipeline
      while (session->persistent() == true &&
	     session->reset_me_ == false &&
	     session->handleio() == false)
	{
	  cout << "[>]stage: " << session->stageid_ << endl;
	  if (modman_.call_hooks(this, session->stageid_, session) == true)
	    modman::next_processing_stage(*session);
	  cout << "[<]stage: " << session->stageid_ << endl;
	}

      // Reset the session, if necessary
      if (session->reset_me_ == true)
	session->reset();

      // go to the next session
      ++cur;
    }

  cout << "exiting pipeline" << endl;

  return true;
}
