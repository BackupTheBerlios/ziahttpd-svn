//
// sockin.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 15:23:07 2005 texane
// Last update Wed Oct 12 18:36:55 2005 texane
//


#ifndef SERVER_SOCKIN_HH
# define SERVER_SOCKIN_HH


#include <sysapi.hh>


namespace server
{
  typedef struct
  {
  } session_param_t;

  typedef struct
  {
    sysapi::socket_in::handle_t hdl_sock_;
    struct sockaddr* sa_sock_;
  } sockin_t;
}


#endif // ! SERVER_SOCKIN_HH
