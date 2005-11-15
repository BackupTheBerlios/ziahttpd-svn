//
// service.hh for  in /home/texane/texane/wip/ept3/ziahttpd/ziahttpd/src/modules/adm
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Nov 14 15:37:39 2005 
// Last update Mon Nov 14 15:40:12 2005 
//


#ifndef SERVER_SERVICE_HH
# define SERVER_SERVICE_HH


#include <string>


namespace server
{
  class service
  {
    // Here are all the services made
    // available for modules.

  public:
    // Debugging purposes only
    virtual void echo(const std::string&);
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

//! \fn virtual void server::service::echo(const std::string& msg)
//! \brief Echo the message
//!
//! \param msg Message the module wants the server to ouput
//! \return no returned value
//!
//! Let the server output a message, testing purpose




#endif // ! SERVER_SERVICE_HH
