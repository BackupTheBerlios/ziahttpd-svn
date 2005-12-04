//
// service.cc for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:45:55 2005 
// Last update Sun Dec 04 17:13:58 2005 texane
//


#include <string>
#include <vector>
#include <iostream>
#include <server/core.hh>
#include <http/session.hh>
#include <server/modman.hh>
#include <server/service.hh>
#include <dataman/buffer.hh>
#include <dataman/conf.hh>
#include <dataman/resource.hh>


using std::string;
using std::vector;
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


// Get a session by id

bool	server::service::find_session_byid(sysapi::socket_in::handle_t& hsock,
					   http::session*& session)
{
  // Found session is
  // stored in session.

  return http::session_manager::find_byhdl(hsock, session);
}


// Io operations

bool	server::service::perform_io(sysapi::socket_in::handle_t& hsock,
				    eventid_t evid,
				    sockioman::sockiohandler_t handler,
				    dataman::buffer* buffer)
{
  // Call the io manager to handle the
  // request.

  bool ret;

  ret = false;

  if (evid == EVREAD)
    ret = core::instance_->ioman_->read(hsock, handler, buffer);
  else if (evid == EVWRITE)
    ret = core::instance_->ioman_->write(hsock, handler, buffer);
  else if (evid == EVCLOSE)
    ret = core::instance_->ioman_->close(hsock, handler);
  else if (evid == EVTIMEOUT)
    ret = core::instance_->ioman_->timeout(hsock, handler);

  // Mark the session as performing an io
  http::session* session;
  if (http::session_manager::find_byhdl(hsock, session) == false)
    {
      cerr << "Session unknown" << endl;
      return false;
    }
  
  session->handleio() = true;
  return true;
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
					 const std::vector<const std::string>& env)
{
  if (session.resource_ != 0)
    return false;

  session.resource_ = dataman::resource::factory(av, env);
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


// In resources creation
bool	server::service::create_resource_in(session& session,
					    sysapi::socket_in::handle_t hsock,
					    sysapi::socket_in::size_t szbody)
{
  if (session.resource_in_ != 0)
    return false;

  session.resource_in_ = dataman::resource::factory(hsock, szbody);
  return true;
}


// Processing stage related
bool	server::service::next_processing_stage(http::session& session)
{
  server::modman::next_processing_stage(session);
  return true;
}


// Configuration related operations

string&		server::service::query_conf_simple(session& session,
						   const string& key)
{
  return session.conf().GetSimpleString(key);
}


vector<string>&	server::service::query_conf_complex(session& session,
						    const string& key)
{
  return session.conf().GetListVector(key);
}
