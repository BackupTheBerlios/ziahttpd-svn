//
// core.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 11 17:27:27 2005 texane
// Last update Mon Nov 14 16:09:44 2005 
//


#ifndef SERVER_CORE_HH
# define SERVER_CORE_HH


#include <string>
#include <server/modman.hh>
#include <server/service.hh>
#include <dataman/conf.hh>
#include <sysapi/sysapi.hh>


namespace server
{
  class core
  {
  public:

    // Initialization
    core();
    core(int, char**);
    ~core();

    // Configuration
    bool reload_conf();
    bool reload_conf(const std::string&);

    // Runtime related
    bool run();

    // Singleton-like behaviour
    static core* instance();

    // Entry point for the processing thread
    static sysapi::thread::retcode_t	process_request(sysapi::thread::param_t);

    // Server core exports services for modules to be able to write, read...
    static service* services_;


  private:

    // Sole instance of the pointer, singleton-like behaviour
    static core* instance_;

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