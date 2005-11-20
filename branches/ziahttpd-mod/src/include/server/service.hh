//
// service.hh for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:37:39 2005 
// Last update Sun Nov 20 16:58:58 2005 texane
//


#ifndef SERVER_SERVICE_HH
# define SERVER_SERVICE_HH


#include <string>


namespace server
{
  class core;
  class service
  {
    // Here are all the services made
    // available for modules.

  public:
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
    virtual bool open_connection(http::session&);
    virtual bool close_connection(http::session&);
    virtual bool write_to_connection(http::session&);
    virtual bool read_from_connection(http::session&);

    // - Conf related operations
    // ?
    
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

//! \fn virtual bool server::service::open_connection(http::session& session)
//! \brief Open a connection for the given port(?)
//!
//! \param session Current session
//! \return success or failure
//!
//! Ask the server to open a new connection, that's creating a listening socket

//! \fn virtual bool server::service::write_to_connection(http::session& session)
//! \brief Write to the session endpart of the connection
//!
//! \param session Current session
//! \return success or failure
//!
//! Ask the server to write the buffer to the session associated endpart of the
//! connection; <b>TEMPORARLY OMIT BUFFER AND SIZE PARAMS, DATAMAN::BUFFER&</b>

//! \fn virtual bool server::service::read_from_connection(http::session& session)
//! \brief Read from the session endpart of the connection
//!
//! \param session Current session
//! \return success or failure
//!
//! Ask the server to read the buffer from the session associated endpart of the
//! connection; <b>TEMPORARLY OMIT BUFFER AND SIZE PARAMS, DATAMAN::BUFFER&</b>

//! \fn virtual bool server::service::close_connection(http::session& session)
//! \brief Close the connection associated to session
//!
//! \param session Current session
//! \return success or failure
//!
//! Ask the server to close the connection associtaed with session



#endif // ! SERVER_SERVICE_HH
