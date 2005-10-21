//
// dataman.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Oct 13 16:08:26 2005 texane
// Last update Fri Oct 21 11:11:27 2005 
//


#ifndef HTTP_DATAMAN_HH
# define HTTP_DATAMAN_HH


#include <sysapi.hh>


namespace http
{
  namespace dataman
  {
    bool get_nextline(sysapi::socket_in::handle_t, char**, sysapi::socket_in::error_t* = NULL);
    bool get_nextblock(sysapi::socket_in::handle_t, unsigned char*, sysapi::socket_in::size_t, sysapi::socket_in::size_t*);
  }
}


#endif // ! HTTP_DATAMAN_HH
