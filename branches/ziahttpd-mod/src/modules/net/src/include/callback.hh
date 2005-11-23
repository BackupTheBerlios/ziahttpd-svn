//
// callback.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 11:42:34 2005 texane
// Last update Wed Nov 23 11:44:11 2005 texane
//


#ifndef CALLBACK_HH
# define CALLBACK_HH


#include <server/service.hh>
#include <http/http.hh>


// Callbacks exported from callback.o
bool read_httpheaders(http::session*, server::service::iovec_t&);
bool read_httpbody(http::session*, server::service::iovec_t&);
bool read_cgistdout(http::session*, server::service::iovec_t&);
bool write_httpresponse(http::session*, server::service::iovec_t&);
bool write_cgistdin(http::session*, server::service::iovec_t&);
bool close_httpconnection(http::session*, server::service::iovec_t&);


#endif // ! CALLBACK_HH
