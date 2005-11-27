//
// module.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 11 17:04:06 2005 texane
// Last update Fri Nov 25 14:29:05 2005 texane
//


#ifndef SERVER_MODULE_HH
# define SERVER_MODULE_HH


#include <string>
#include <http/session.hh>
#include <sysapi/sysapi.hh>


namespace server
{
  // Forward declarations
  class core;

  class module
  {
  public:

    // Construction/destruction
    module(const std::string&, bool, bool);
    ~module();

    // Internal management
    bool reset();

    // Shared object related
    sysapi::shared_object::handle_t hobj_;

    // Status returned by the module
    typedef enum
    {
    } reason_t;

    // Module exported hooks
    typedef bool (*hook_t)(http::session&, server::core*, int& reason);
    
    hook_t hk_create_con_;		// for ssl

    hook_t hk_get_rqstmetadata_;	// http status + header lines
    hook_t hk_get_rqstdata_;		// http body (post...)
    hook_t hk_parse_rqstmetadata_;	// http protocol parser, may be version 1.0, 1.1, ftp...
    hook_t hk_alter_rqstdata_;		// unzipping(mod_uncompress...), name translation(mod_alias, mod_mime)...

    hook_t hk_build_respmetadata_;	// build status line, header lines, ftp header...
    hook_t hk_build_respdata_;		// http get from cgi, local file, byfly generation, ftp from local file...
    hook_t hk_alter_respdata_;		// mod_php...
    hook_t hk_alter_respmetadata_;	// Set the content-length, encoding(?), specific header modules

    hook_t hk_send_response_;		// for ssl
    hook_t hk_release_con_;		// for ssl

    // Module informations
    typedef enum {} role_t;
    typedef enum {} statecode_t;

    std::string name_;
    bool priviledged_;
    role_t role_;
    statecode_t stcode_;
    bool running_;

  };
}


//! \class server::module
//! \brief Modules implement server functionalities extension.
//!
//! Modules register hooks to be called at different stages of the
//! request processing flow.
//! The flow is broken into X stages.
//! Here is a detailed explaination of the steps involved:
//!  -# Create a new internet socket for the incoming connection
//!	-# <b>CON_CREATION HOOK</b>: For ssl module, a special socket is to be created
//!  -# Read data from the socket
//!	-# <b>GET_RQSTMETADATA_HOOK</b>: For ssl module, a special read function
//!	-# <b>GET_RQSTADATA_HOOK</b>
//!  -# Actually process the request
//!	-# <b>PARSE_REQUEST_METADATA_HOOK</b>: (in the case we are not dealing with proto)
//!	-# <b>ALTER_REQUEST_DATA_HOOK</b>: (for mod_alias, mod_mime...)
//!  -# Build response, including metadata building and content generation
//!	-# <b>BUILD_RESPMETADATA_HOOK</b>: Construct response status and header lines...
//!	-# <b>BUILD_RESPDATA_HOOK</b>: cgi execution, go reading a file on disk, generate error pages...
//!  -# Last chance to alter repsonse before it is sent to client
//!	-# <b>ALTER_RESPDATA_HOOK</b>: A previously loaded module might want to see how the response was modified by other after it has processed it
//!	-# <b>ALTER_RESPMETADATA_HOOK</b>
//!  -# Send the response to the client
//!	-# <b>SEND_RESPONSE_HOOK</b>
//!  -# Release the session connection
//!	-# <b>CON_RELEASING_HOOK</b>
//! Other notes on modules:
//! -# There can be multiple hooks registered for a given stage, allowing hook chaining.
//! -# Non priviledged modules can only access the current session, containing informations
//! about the current request (buffer, internal representation, accessed resource...).
//! -# Priviledged modules can access the server core internals.
//! -# Modules have dependencies, handled at load time.
//! -# Modules may have roles (?)
//! -# Modules can be in a running state or not


//! \fn bool server::modman::load_at_beginning(const std::string& id,
//!					       bool priviledged = false,
//!					       bool activ = true)
//! \brief load a module at the beginning of the list.
//!
//! \param id Path identifying the module
//! \param priviledged wether or not the module is a priviledged one
//! \param activ is the module activated at loading
//! \return false on error (either the module is not found, permission denied...).
//!
//! Load a module at the beginning of the modlist_.


#endif // ! SERVER_MODULE_HH
