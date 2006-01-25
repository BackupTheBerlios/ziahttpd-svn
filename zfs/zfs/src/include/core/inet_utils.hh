//
// inet_utils.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Jan 24 21:44:18 2006 texane
// Last update Tue Jan 24 22:25:36 2006 texane
//


#ifndef INET_UTILS_HH
# define INET_UTILS_HH


#include <windows.h>
#include <string>


// Internet socket helper functions

namespace inet_utils
{
  bool set_nonblocking_socket(int&);
  bool create_listening_socket(int&, struct sockaddr_in&, unsigned short, unsigned long, int);
  bool create_listening_socket(int&, struct sockaddr_in&, unsigned short, std::string&, int);
  bool init_subsystem();
  bool release_subsystem();
  bool accept(int&, struct sockaddr_in&, int&);
}


#endif // ! INET_UTILS_HH
