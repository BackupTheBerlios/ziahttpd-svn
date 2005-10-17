//
// dataman.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Oct 13 16:08:26 2005 texane
// Last update Thu Oct 13 18:59:02 2005 texane
//


#ifndef HTTP_DATAMAN_HH
# define HTTP_DATAMAN_HH


#include <sysapi.hh>


namespace http
{
  namespace dataman
  {
    bool get_nextline(sysapi::socket_in::handle_t, char**);
    bool get_nextblock(sysapi::socket_in::handle_t,
		       unsigned char**,
		       sysapi::socket_in::size_t*);
  }
}


#endif // ! HTTP_DATAMAN_HH
