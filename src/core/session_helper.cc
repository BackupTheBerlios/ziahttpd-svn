//
// session_helper.cc for  in /home/texane/texane/wip/ept3/zia_current/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Thu Oct 20 19:28:39 2005 
// Last update Sat Oct 22 16:13:11 2005 texane
//


// This file implements helper functions
// for the session class.


#include <cstring>
#include <server.hh>


bool	server::session::get_statusline(char** ptrline, sysapi::socket_in::error_t* err)
{
  bool	ret;
  char*	line;

  while ((ret = http::dataman::get_nextline(hdl_con_, &line, err)) == true && !::strlen((const char*)line))
    delete[] line;

  *ptrline = line;
  return ret;
}


bool	server::session::get_headerline(char** ptr_line, sysapi::socket_in::error_t* err)
{
  return http::dataman::get_nextline(hdl_con_, ptr_line, err);
}


bool	server::session::get_body(unsigned char** buf, sysapi::socket_in::size_t* sz_body, sysapi::socket_in::error_t* err)
{
  if (http_info_.is_body_ == true)
    return http::dataman::get_nextblock(hdl_con_, buf, http_info_.sz_body_, sz_body, err);
  return false;
}
