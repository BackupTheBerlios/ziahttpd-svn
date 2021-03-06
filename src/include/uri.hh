//
// uri.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:46:22 2005 
// Last update Tue Nov  8 13:07:55 2005 
//


#ifndef HTTP_DATAMAN_URI_HH
# define HTTP_DATAMAN_URI_HH


#include <string>


// Name a resource from the client stand point
// For the server standpoint, tell the actual location
// of the resource.
// The only goal of this class is to map the universal name
// to a local one ACCORDING TO THE CONFIGURATION FILE.
// In order to do so, it must take a configuration as argument.
// Access checks is delegated to the resource class.

namespace http
{
  namespace dataman
  {
    class uri
    {
    public:
      uri();
      uri(const std::string&);
      ~uri();

      std::string&	universal_name();
      std::string&	local_name();

      static std::string map_universal_to_local(const std::string&);
      static std::string map_local_to_universal(const std::string&);

    private:
      std::string	universal_name_;
      std::string	local_name_;

      // const config& conf_;
    };
  }
}


//! \file
//! \brief unique resource identifier class
//!
//! Name a resource from the client stand point
//! For the server standpoint, tell the actual location
//! of the resource.
//! The only goal of this class is to map the universal name
//! to a local one ACCORDING TO THE CONFIGURATION FILE.
//! In order to do so, it must take a configuration as argument.
//! Access checks is delegated to the resource class.

//! \class http::dataman::uri
//! \brief unique resource identifier class
//!
//! Name a resource from the client stand point
//! For the server standpoint, tell the actual location
//! of the resource.
//! The only goal of this class is to map the universal name
//! to a local one ACCORDING TO THE CONFIGURATION FILE.
//! In order to do so, it must take a configuration as argument.
//! Access checks is delegated to the resource class.

//! \fn string& http::dataman::uri::universal_name()
//! \brief Return a string containing the universal name for the resource.
//! 

//! \fn string& http::dataman::uri::local_name()
//! \brief Return a string containing the local name for the resource.
//! 


#endif // ! HTTP_DATAMAN_URI
