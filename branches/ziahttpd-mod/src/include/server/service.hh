//
// service.hh for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:37:39 2005 
// Last update Sun Dec 04 19:34:02 2005 texane
//


#ifndef SERVER_SERVICE_HH
# define SERVER_SERVICE_HH


#include <string>
#include <vector>
#include <http/session.hh>
#include <dataman/buffer.hh>
#include <server/sockioman.hh>


namespace server
{
  class core;
  class service
  {
    // Here are all the services made
    // available for modules.

  public:
//     // - Singleton like behaviour
//     static service* instance_;

    // - Log/Debug related operations
    virtual void echo(const std::string&);

    // - Module related operations
    // (?) should be priviledged operations,
    // pass the address of the core as a token
    // identifier. Access are allowed if:
    // token == coreaddr
    typedef server::core* security_token_t;
    virtual bool load_module(const security_token_t&,
			     const std::string&,
			     const std::string&,
			     const std::string&);
    virtual bool unload_module(const security_token_t&,
			       const std::string&);
    virtual bool stat_module(const security_token_t&,
			     const std::string&);


    // - Network io related operations
    // This mechanism might appear to be somewhat complex
    // (for instance comparing with a single write() call);
    // However, this is necessary in order for the server to
    // hide its implementation details; For instance, it can
    // use iomultiplexing or even simple blocking calls, this
    // doesn't matter for modules.
    // - Information passing between modules and server
    // iovec_t, information passing between server and module
    // In order for the server and the module(client) to communicate
    // information (completion error code, buffers...), the api defines
    // iovec_t to be the auxilliary structure containing necessary datas.

    // ?
    // The function should receive an error argument
    // in order for the core server to communicate
    // the module with the operation completion status.
    // More generally, we should define a type for handle
    // operation status for core <-> module communication.

    // Register callbacks on events
    typedef enum
      {
	EVREAD = 0,
	EVWRITE,
	EVCLOSE,
	EVTIMEOUT
      } eventid_t;


    // Retrieve a session by id (socket handle...)
    virtual bool find_session_byid(sysapi::socket_in::handle_t&,
				   http::session*&);

    // Io related operations.
    // ? (if someone out there knows how to have virtual templated methods...)
    // For read and write operations
    virtual bool perform_io(sysapi::socket_in::handle_t&,
			    eventid_t,
			    sockioman::sockiohandler_t,
			    dataman::buffer* = 0);
    virtual bool get_nextline(sysapi::socket_in::handle_t,
			      char**,
			      sysapi::socket_in::error_t* = NULL);
    virtual bool get_nextblock(sysapi::socket_in::handle_t,
			       unsigned char**,
			       sysapi::socket_in::size_t,
			       sysapi::socket_in::size_t*,
			       sysapi::socket_in::error_t* = NULL);

    // - Resource creation related services
    virtual bool create_resource(http::session&, const std::string&);
    virtual bool create_resource(http::session&, const std::vector<const std::string>&,
				 const std::vector<const std::string>&);
    virtual bool create_resource(http::session&, unsigned int);
    virtual bool create_resource_in(http::session&,
				    sysapi::socket_in::handle_t,
				    sysapi::socket_in::size_t);

    
    // - Request processing stage related services
    virtual bool next_processing_stage(http::session&);


    // ?
    // - Conf related operations
    virtual std::string& query_conf_simple(http::session&, const std::string&);
    virtual std::vector<std::string>& query_conf_complex(http::session&, const std::string&);
  };
}


//! \class server::service
//! \brief Services exported by the server to modules
//!
//! Modules have to perform actions when processing data;
//! Some of those actions need interactions with server internals,
//! for instance io related operation.
//! In order to do so, the server export services to modules by the way
//! of the service class.
//! <b>TODOLIST: (zihappy members involved here)</b>
//! -# Add a way to communicate error code between services and modules
//! -# See for service method names
//! -# Define configuration access
//! -# Don't use dataman::buffer instead of unsigned char*
//! -# Don't use sysapi::socket_in::handle_t
//! -# Think about a callback system for the server to communicate with the module
//! -# Defines an information vector for module stating (modstat_t)

//! \fn virtual void server::service::echo(const std::string& msg)
//! \brief Echo the message
//!
//! \param msg Message the module wants the server to ouput
//! \return no returned value
//!
//! Let the server output a message, testing purpose

//! \fn virtual bool server::service::load_module(const server::service::security_token_t& tok,
//!						  const std::string& after,
//!						  const std::string& target,
//!						  const std::string& before)
//! \brief load the module between and after another ones.
//!
//! \param tok Security token passed by the server to module at hook call time
//! \param after Name of the module after which I wanna be loaded
//! \param target The name of the module to be loaded
//! \param before Name of the module before which I wanna be loaded
//! \return True if the module is loaded, false otherwise.
//!
//! Load the module after and before other ones. This operation can be denied
//! by the server if the security token is invalid.

//! \fn virtual bool server::service::unload_module(const server::service::security_token_t& tok,
//!						   const std::string& target)
//! \param tok Security token passed by the server to module at hook call time
//! \brief Unload the module
//!
//! \param target Name of the module to be unloaded
//! \return True if the module has been unloaded, false otherwise.
//!
//! Unload the module. Security token is not yet implemented, but see the above describtion.

//! \fn virtual bool server::service::stat_module(const server::service::security_token_t& tok,
//!						  const std::string& target)
//! \brief Stat the module
//!
//! \param target Name of the module to be stated.
//! \return False if the module cannot be found(the module has not been loaded); Else return true.
//!
//! Stat the module; <b>Stat structure has not yet been defined</b>

//! \enum sever::service::eventid_t
//!
//! \brief Event identifiers

//! \fn virtual bool server::service::register_callback(sysapi::socket_in::handle_t& hsock,
//!							server::service::eventid_t evid,
//!							server::service::callback_t& cb)
//! \brief Register a callback to be called when event evid occures.
//!
//! \param hsock Handle of the internet socket on which to perform operation
//! \param evid Identifer of the event
//! \param cb Address of the function to be called when event occures
//! \return Return true on success, false else (?)
//!
//! Ask the server to register a callback for the given event on a given socket handle
//! @see the header file for a list of event identifiers

//! \fn virtual bool server::service::perform_io(sysapi::socket_in::handle_t& hsock,
//!						 server::service::eventid_t evid,
//!						 server::service::iovec_t& iov)
//! \brief Tell the server to perform an io operation
//!
//! \param hsock Handle of the internet socket on which to perform operation
//! \param evid Identifer of the event
//! \param iov Vector for information passing between module and server
//! \return Return false if the event has no registered callback.
//!
//! Ask the server to perform an operation.
//! -# Network io related operations
//! This mechanism might appear to be somewhat complex
//! (for instance comparing with a single write() call);
//! However, this is necessary in order for the server to
//! hide its implementation details; For instance, it can
//! use iomultiplexing or even simple blocking calls, this
//! doesn't matter for modules.
//! -# Information passing between modules and server
//! iovec_t, information passing between server and module
//! In order for the server and the module(client) to communicate
//! information (completion error code, buffers...), the api defines
//! iovec_t to be the auxilliary structure containing necessary datas.


#endif // ! SERVER_SERVICE_HH
