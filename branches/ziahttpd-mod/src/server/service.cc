//
// service.cc for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:45:55 2005 
// Last update Tue Nov 22 11:00:23 2005 texane
//


#include <string>
#include <iostream>
#include <server/core.hh>
#include <http/session.hh>
#include <server/modman.hh>
#include <server/service.hh>
#include <dataman/buffer.hh>
#include <dataman/conf.hh>


using std::string;
using std::cout;
using std::endl;
using http::session;
using dataman::buffer;


// Simple logging service

void	server::service::echo(const string& what)
{
  cout << "[*] From the echo service: " << what << endl;
}


// {Load/Unload/Stat} modules services

bool	server::service::load_module(const security_token_t& token,
				     const string& before,
				     const string& target,
				     const string& after)
{
  cout << "[!] Loading module: " << target
       << "with token id 0x" << std::hex
       << token << endl;

  return core::instance()->modman_.load_at_beginning(target);
}

bool	server::service::unload_module(const security_token_t& token,
				       const string& target)
{
  cout << "[!] Loading module: " << target
       << "with token id 0x" << std::hex
       << token << endl;

  return core::instance()->modman_.unload(target);
}

bool	server::service::stat_module(const security_token_t& token,
				     const string& target)
{
  cout << "[!] Loading module: " << target
       << "with token id 0x" << std::hex
       << token << endl;

  cout << "[!] SERVICE NOT YET IMPLEMENTED BY SERVER" << endl;

  return false;
}


// Current session {Connection, Io} related services
// Pass the whole session to callback'ed functions
// so that they can store module-related error codes

// For the moment, callbacks handling is implemented
// by an array of callbacks

static server::service::callback_t callbacks_[4] = {0, 0, 0, 0};


// Callback registering services

bool	server::service::register_callback(sysapi::socket_in::handle_t& hsock,
					   server::service::eventid_t evid,
					   const server::service::callback_t& cb)
{
  // @see service.hh for a list callbacks
  // supported by the server.
  // This function erase the old callback, if any,
  // and always returns true.

  callbacks_[evid] = cb;
  return true;
}


// Io operations

bool	server::service::perform_io(sysapi::socket_in::handle_t& hsock,
				    server::service::eventid_t evid,
				    server::service::iovec_t& iov)
{
  // Perform io operation according by calling the
  // callback registered for evid event.
  // Return false if no callbac is registered.

  if (callbacks_[evid] == 0)
    return false;

  // !
  // Currently, no dereferd called is done, blocking mode.
  // This is temporary, since the iomanager will come soon.
  return callbacks_[evid](hsock, iov);
}


// Configuration related operations

string		server::service::query_conf(session& session, const string& key)
{
  // Temporary function
  return session.conf().GetSimpleString(key);
}
