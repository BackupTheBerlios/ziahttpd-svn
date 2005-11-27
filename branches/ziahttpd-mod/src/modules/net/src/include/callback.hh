//
// callback.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 11:42:34 2005 texane
// Last update Sun Nov 27 00:33:41 2005 texane
//


#ifndef CALLBACK_HH
# define CALLBACK_HH


#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>
#include <server/service.hh>
#include <http/http.hh>


// Callbacks exported from callback.o

bool read_metadata(sysapi::socket_in::handle_t&,
		   dataman::buffer*,
		   sysapi::socket_in::error_t&);
bool read_data(sysapi::socket_in::handle_t&,
	       dataman::buffer*,
	       sysapi::socket_in::error_t&);
bool send_response(sysapi::socket_in::handle_t&,
		   dataman::buffer*,
		   sysapi::socket_in::error_t&);
bool close_connection(sysapi::socket_in::handle_t&,
		      dataman::buffer*,
		      sysapi::socket_in::error_t&);


#endif // ! CALLBACK_HH
