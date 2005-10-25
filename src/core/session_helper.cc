//
// session_helper.cc for  in /home/texane/texane/wip/ept3/zia_current/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Thu Oct 20 19:28:39 2005 
// Last update Tue Oct 25 22:55:51 2005 
//


// This file implements helper functions
// for the session class.


#include <iostream>
#include <string>
#include <cstring>
#include <server.hh>
#include <buffer.hh>


using std::string;


bool	server::session::get_statusline(http::dataman::buffer& buf, sysapi::socket_in::error_t* err)
{
  bool	ret;
  char*	line;

  while ((ret = http::dataman::get_nextline(hdl_con_, &line, err)) && !strlen((const char*)line))
    {
      std::cout << "GOT STATUS LINE" << std::endl;
      delete[] line;
    }
  if (!ret) {       std::cout << "GOT STATUS LINE FALSE" << std::endl; return false;}
  
  buf = (const string)line;
  return ret;  
}


bool	server::session::get_headerline(http::dataman::buffer& buf, sysapi::socket_in::error_t* err)
{
  char* line;
  bool ret;

  ret = http::dataman::get_nextline(hdl_con_, &line, err);
  if (ret == true)
    buf = (const string)line;
  else
    buf.reset();

  return ret;
}


bool	server::session::get_body(http::dataman::buffer& buf, sysapi::socket_in::error_t* err)
{
  bool ret;
  sysapi::socket_in::size_t sz;

  ret = false;

//   if (http_info_.is_body_ == true)
  if (buf.size())
    {
      unsigned char* ptr;
      ret = http::dataman::get_nextblock(hdl_con_, &ptr, buf.size(), &sz, err);
      if (ret == true)
	{
	  buf.buf(ptr, (size_t)sz);
	  std::cout << "EXE" << std::endl;
	  buf.display();
	  delete[] ptr;
	}
    }

  return ret;
}
