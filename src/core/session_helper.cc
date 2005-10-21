//
// session_helper.cc for  in /home/texane/texane/wip/ept3/zia_current/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Thu Oct 20 19:28:39 2005 
// Last update Fri Oct 21 12:47:22 2005 
//


// This file implements helper functions
// for the session class.


#include <cstring>
#include <server.hh>


bool	server::session::skip_crlf(char** ptr_line, sysapi::socket_in::error_t* err)
{
  bool	ret;
  char*	line;

  while ((ret = http::dataman::get_nextline(hdl_con_, &line, err)) == true && !::strlen((const char*)line))
    delete[] ptr_line;

  *ptr_line = line;
  return ret;
}


bool	server::session::get_headerlines(char** ptr_line, sysapi::socket_in::error_t* err)
{
  return http::dataman::get_nextline(hdl_con_, ptr_line, err);
}


bool	server::session::get_body(unsigned char** buf, sysapi::socket_in::size_t sz, sysapi::socket_in::size_t* nr_recv, sysapi::socket_in::error_t* err)
{
  bool ret;
  sysapi::socket_in::size_t nr_read;

  *nr_recv = 0;

  ret = http::dataman::get_nextblock(hdl_con_, buf, sz, &nr_read);

  if (nr_read < sz)
    {
      ret = sysapi::socket_in::recv(hdl_con_, (*buf) + nr_read, sz - nr_read, nr_recv);
      if (ret == false)
	delete *buf;
    }

  *nr_recv += nr_read;

  return ret;
}


bool	server::session::get_body(unsigned char** buf, sysapi::socket_in::size_t* sz, sysapi::socket_in::size_t* nr_recv, sysapi::socket_in::error_t* err)
{
  return false;
}
