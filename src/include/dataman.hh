//
// dataman.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Oct 13 16:08:26 2005 texane
// Last update Fri Oct 21 12:29:20 2005 
//


#ifndef HTTP_DATAMAN_HH
# define HTTP_DATAMAN_HH


#include <sysapi.hh>


namespace http
{
  namespace dataman
  {
    // Get the next http line
    bool get_nextline(sysapi::socket_in::handle_t,
		      char**,
		      sysapi::socket_in::error_t* = NULL);

    // Get the next block
    bool get_nextblock(sysapi::socket_in::handle_t,
		       unsigned char**,
		       sysapi::socket_in::size_t,
		       sysapi::socket_in::size_t*,
		       sysapi::socket_in::error_t* = NULL);
  }
}


#endif // ! HTTP_DATAMAN_HH
