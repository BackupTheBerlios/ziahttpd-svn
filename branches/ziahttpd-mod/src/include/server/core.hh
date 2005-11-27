//
// core.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 11 17:27:27 2005 texane
// Last update Sun Nov 27 00:50:19 2005 texane
//


#ifndef SERVER_CORE_HH
# define SERVER_CORE_HH


#include <list>
#include <string>
#include <server/modman.hh>
#include <server/service.hh>
#include <dataman/conf.hh>
#include <server/sockioman.hh>
#include <sysapi/sysapi.hh>
#include <http/session.hh>


namespace server
{
  class core
  {
  public:

    friend class service;

    // Initialization
    core();
    core(int, char**);
    ~core();

    // Configuration
    bool reload_conf();
    bool reload_conf(const std::string&);

    // - temporary
    dataman::conf& conf()
    { return conf_; }

    // Handle default connection
    static bool handle_default_connection(sysapi::socket_in::handle_t&,
					  dataman::buffer*,
					  sysapi::socket_in::error_t&);

    // Runtime related
    bool run();

    // Singleton-like behaviour
    static core* instance();

    // Entry point for the processing thread
    static sysapi::thread::retcode_t	process_request(sysapi::thread::param_t);

    // Server core exports services for modules to be able to write, read...
    static service* services_;

    // Sessions releated services
    // ? Maybe there should be a session factory,
    // more generally a session manager.
    bool register_session(const sysapi::socket_in::handle_t&);
    bool unregister_session(const sysapi::socket_in::handle_t&);
    bool find_session_byhdl(const sysapi::socket_in::handle_t&,
			    http::session*&);
    bool process_sessions();


  private:

    // Sole instance of the pointer, singleton-like behaviour
    static core* instance_;

    // List of session
    std::list<http::session*> sessions_;

    // Socket io manager
    sockioman* ioman_;

    // Server core status related
    bool done_;
    bool initialized_;

    dataman::conf conf_;

    // Module management related
    modman modman_;

    // Internal data management methods
    bool reset();
    bool release();
  };
}


//! \class server::core
//! \brief Server core
//!
//! The zia http server is a modular one.
//! As Apache, the server is divided into modules,
//! so that the core is very minimalistic.
//! It contains io, thread and module managers, where as
//! the two first ones could be exported...


#endif // ! SERVER_CORE_HH
