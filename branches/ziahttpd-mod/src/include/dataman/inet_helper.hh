//
// inet_helper.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 13:46:44 2005 
// Last update Sun Nov 13 14:29:42 2005 
//


#ifndef DATAMAN_INET_HELPER_HH
# define DATAMAN_INET_HELPER_HH


#include <sysapi/sysapi.hh>


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


#endif // ! DATAMAN_INET_HELPER_HH
