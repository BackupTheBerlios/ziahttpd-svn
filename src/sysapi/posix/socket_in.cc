//
// socket_in.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:40:35 2005 
// Last update Mon Oct 17 18:44:36 2005 
//


#include <posix.hh>


bool posix::socket_in::init_subsystem(error_t*)
{
  return false;
}


bool posix::socket_in::release_subsystem(error_t*)
{
  return false;
}


bool posix::socket_in::create_listening(handle_t*,
					unsigned short,
					unsigned long,
					int,
					error_t*)
{
  return false;
}


bool posix::socket_in::create_listening(handle_t*,
					unsigned short,
					const char*,
					int,
					error_t*)
{
  return false;
}


bool posix::socket_in::accept(handle_t*,
			      handle_t,
			      struct sockaddr*,
			      error_t*)
{
  return false;
}


bool posix::socket_in::terminate_connection(handle_t)
{
  return false;
}


bool posix::socket_in::recv(handle_t,
			    unsigned char*,
			    size_t,
			    size_t*,
			    error_t*)
{
  return false;
}


bool posix::socket_in::send(handle_t,
			    const unsigned char*,
			    size_t,
			    size_t*,
			    error_t*)
{
  return false;
}
