//
// service.cc for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:45:55 2005 
// Last update Tue Nov 22 18:34:27 2005 texane
//


#include <string>
#include <iostream>
#include <server/core.hh>
#include <http/session.hh>
#include <server/modman.hh>
#include <server/service.hh>
#include <dataman/buffer.hh>


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

// Callback registering services

bool	server::service::register_callback(sysapi::socket_in::handle_t& hsock,
					   server::service::eventid_t evid,
					   server::service::callback_t& cb)
{
  // @see service.hh for a list callbacks
  // supported by the server.
  
//   if (callbacks_[reinterpret_cast<unsigned int>(evid)] == 0)
//     {
//       return true;
//     }

//   callbacks_[reinterpret_cast<unsigned int>(evid)].cb_ = cb;

  return false;
}


// Io operations

bool	server::service::perform_io(sysapi::socket_in::handle_t& hsock,
				    server::service::eventid_t evid,
				    server::service::iovec_t& iov)
{
  // Perform operation related to
  // io (read, write, timeout, close)

//   if (callbacks_[reinterpret_cast<unsigned int>(evid)] == 0)
//     {
//       return false;
//     }
  
  return true;
}
