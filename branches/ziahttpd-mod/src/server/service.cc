//
// service.cc for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:45:55 2005 
// Last update Sun Nov 20 18:25:37 2005 texane
//


#include <string>
#include <iostream>
#include <server/core.hh>
#include <http/session.hh>
#include <server/modman.hh>
#include <server/service.hh>


using std::string;
using std::cout;
using std::endl;
using http::session;


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

bool	server::service::open_connection(session&)
{
  return false;
}

bool	server::service::close_connection(session&)
{
  return false;
}

bool	server::service::write_to_connection(session&)
{
  return false;
}

bool	server::service::read_from_connection(session&)
{
  return false;
}
