//
// sockioman.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 25 20:01:32 2005 texane
// Last update Sun Nov 27 00:55:13 2005 texane
//


#ifndef SERVER_SOCKIOMAN_HH
# define SERVER_SOCKIOMAN_HH


#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>


namespace server
{}


namespace thrman = server;


namespace server
{
  class http::session;

  class sockioman
  {
    // - Sockio manager interface

  public:

    // Io handlers are the functions called when data
    // is to be read/write from/to the socket handle,
    // or when the connection associated with the handle
    // timeout or is closed by one end.

    // Later, the callback function is invoked with this type
    typedef bool (*sockiohandler_t)(sysapi::socket_in::handle_t&,
				    dataman::buffer*,
				    sysapi::socket_in::error_t&);
    typedef struct sockio
    {
      bool done_;
      bool used_;

      // Socket related
      sysapi::socket_in::handle_t hsock_;

      // Buffers
      dataman::buffer* rdbuf_;
      dataman::buffer* wrbuf_;

      // Callbacks
      sockiohandler_t onread_;
      sockiohandler_t onwrite_;
      sockiohandler_t onclose_;
      sockiohandler_t ontimeout_;

      // Related iomanager
      sockioman* ioman_;

    } sockio_t;

    // Interface

    // Socket handles registering functnions
    virtual bool register_sockhdl(sysapi::socket_in::handle_t&) = 0;
    virtual bool unregister_sockhdl(sysapi::socket_in::handle_t&) = 0;

    // Operation related functions
    // Later, the callback will be
    // invoked with those params.
    virtual bool write(sysapi::socket_in::handle_t&,
		       sockiohandler_t,
		       dataman::buffer*) = 0;
    virtual bool read(sysapi::socket_in::handle_t&,
		      sockiohandler_t,
		      dataman::buffer*) = 0;
    virtual bool close(sysapi::socket_in::handle_t&,
		       sockiohandler_t) = 0;
    virtual bool timeout(sysapi::socket_in::handle_t&,
			 sockiohandler_t) = 0;

    // Tell the iomanager to handle io pending operation
    virtual bool handle_io() = 0;
    virtual bool available_slots(int&) = 0;
    virtual bool find_sockio_byhdl(sockio_t*&,
				   sysapi::socket_in::handle_t) = 0;
    virtual bool done() = 0;

    // Generic function, should be moved in sockioman class
    static bool generic_onread(struct sockio&);
    static bool generic_onclose(struct sockio&);
  };
}


#endif // ! SERVER_SOCKIOMAN_HH
