//
// service.cc for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:45:55 2005 
// Last update Wed Nov 23 16:41:46 2005 texane
//


#include <string>
#include <iostream>
#include <server/core.hh>
#include <http/session.hh>
#include <server/modman.hh>
#include <server/service.hh>
#include <dataman/buffer.hh>
#include <dataman/conf.hh>
#include <dataman/resource.hh>


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

// Currently, don't use iomanager to store
// informations... use a little array, waiting
// for the iomanager to be done.
// For the moment, only handle on request at a time,
// so let be only one slot in the ioslots_.

#define NIOSLOTS	1
static struct ioslot
{
  server::service::callback_t callbacks_[4];
  server::service::iovec_t iov_;
  http::session* session_;
} ioslots_[NIOSLOTS];


// Callback registering services

bool	server::service::register_callback(http::session& session,
					   server::service::eventid_t evid,
					   const server::service::callback_t& cb)
{
  // @see service.hh for a list callbacks
  // supported by the server.
  // This function erase the old callback, if any,
  // and always returns true.

  ioslots_[0].callbacks_[evid] = cb;
  ioslots_[0].session_ = &session;
  return true;
}


// Io operations

bool	server::service::perform_io(http::session& session, server::service::eventid_t evid)
{
  // Perform io operation according by calling the
  // callback registered for evid event.
  // Return false if no callbac is registered.

  if (ioslots_[0].callbacks_[evid] == 0)
    return false;

  // !
  // Currently, no dereferd called is done, blocking mode.
  // This is temporary, since the iomanager will come soon.
  return ioslots_[0].callbacks_[evid](ioslots_[0].session_, ioslots_[0].iov_);
}



// Resource creation service

bool	server::service::create_resource(session& session,
					 const string& filename)
{
  if (session.resource_ != 0)
    return false;

  session.resource_ = dataman::resource::factory(filename);
  return true;
}

bool	server::service::create_resource(session& session,
					 const std::vector<const std::string>& av,
					 const std::vector<const std::string>& env,
					 const dataman::buffer& buf_stdin)
{
  if (session.resource_ != 0)
    return false;

  session.resource_ = dataman::resource::factory(av, env, buf_stdin);
  return true;
}

bool	server::service::create_resource(session& session,
					 unsigned int stcode)
{
  if (session.resource_ != 0)
    return false;

  session.resource_ = dataman::resource::factory(stcode);
  return true;
}



// Configuration related operations

string		server::service::query_conf(session& session,
					    const string& key)
{
  // Temporary function
//   return session.conf().GetSimpleString(key);
  return "666";
}
