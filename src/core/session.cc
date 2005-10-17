//
// session.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 14:15:31 2005 texane
// Last update Mon Oct 17 18:04:25 2005 
//


#include <cstring>
#include <iostream>
#include <sysapi.hh>
#include <session.hh>
#include <channel.hh>
#include <dataman.hh>


// Session execution entry point
sysapi::thread::retcode_t server::session::entrypoint_(sysapi::thread::param_t param)
{
  sysapi::socket_in::handle_t hdl_accept;
  sysapi::socket_in::handle_t hdl_con;
  char* ptr_line;
  bool ret;

  // Set the listening socket
  if (sysapi::socket_in::create_listening(&hdl_accept, 40000) == false)
    sysapi::error::stringify("cannot create listening: ");
  if (sysapi::socket_in::accept(&hdl_con, hdl_accept) == false)
    sysapi::error::stringify("cannot accept: ");

  sysapi::thread::say("Servicing new request");

  // Skip the crlf, if any
  while ((ret = http::dataman::get_nextline(hdl_con, &ptr_line)) == true && !strlen((const char*)ptr_line))
    {
      std::cout << "<SKIPING_CRLF>" << std::endl;
      delete[] ptr_line;
    }

  if (ret == false)
    {
      sysapi::error::stringify("error http request requestline: ");
      return 0;
    }

  // Get the request line
  std::cout << "<REQUEST_LINE>:" << ptr_line << std::endl;

  // Get message headers and the final crlf
  while ((ret = http::dataman::get_nextline(hdl_con, &ptr_line)) && strlen((const char*)ptr_line))
    {
      std::cout << "<MESSAGE_HEADER>:" << ptr_line << std::endl;
      std::cout << "<HEADER_LEN>:" << (int)strlen(ptr_line) << std::endl;
      delete[] ptr_line;
    }

  if (ret == false)
    {
      sysapi::error::stringify("error http request msgheader: ");
      return 0;
    }

  // We have had a CRLF token
  std::cout << "<CR_LF>" << std::endl;

  // Get the body, if any
  std::cout << "<MESSAGE_BODY>: (if any)" << std::endl;
  // get_body();

  // Terminate  all connections
  sysapi::socket_in::terminate_connection(hdl_accept);
  sysapi::socket_in::terminate_connection(hdl_con);

  return 0;
}


// Session construction
server::session::session()
{
}


// Session destruction
server::session::~session()
{
}
