//
// modman.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 11 15:54:10 2005 texane
// Last update Mon Nov 14 13:13:01 2005 
//


#ifndef SERVER_MODMAN_HH
# define SERVER_MODMAN_HH


#include <list>
#include <string>
#include <http/session.hh>
#include <server/module.hh>


namespace server
{
  // Forward declarations

  class modman
  {
  public:

    modman();
    
    // Module loading
    bool is_loaded(const std::string&,
		   std::list<module*>::iterator* = NULL);
    bool load_at_beginning(const std::string&,
			   bool priviledged = false,
			   bool activ = true);
    bool load_at_end(const std::string&,
		     bool priviledged = false,
		     bool activ = true);
    bool load(const std::string&,
	      const std::string&,
	      const std::string&,
	      bool = false,
	      bool = true,
	      bool = true);
    bool reload(const std::string&,
		bool priviledged = false,
		bool activ = true);

    // Module unloading
    bool unload(const std::string&);

    // Running state management
    bool start(const std::string&);
    bool stop(const std::string&);
    bool state(const std::string&, int&);

    // Module invokation
    typedef enum
      {
	CREATE_CON = 0,
	READ_RQST_METADATA,
	READ_RQST_DATA,
	PARSE_RQST_METADATA,
	ALTER_RQST_DATA,
	BUILD_RESP_METADATA,
	BUILD_RESP_DATA,
	ALTER_RESP_DATA,
	ALTER_RESP_METADATA,
	SEND_RESP,
	RELEASE_CON
      } stageid_t;
    bool call_hooks(core*, stageid_t, http::session*);

    // Conveniences
    module* operator[](const std::string&);

    // Singleton-like behaviour
    static modman* instance();


  private:
    std::list<module*> modlist_;

    // Singleton-like behaviour
    static modman* instance_;
    
  };
}


//! \class server::modman
//! \brief Module manager
//!
//! Handle module management.
//! Provide methods to handle module dependencies,
//! cold module reloading/unloading, running related function.
//! Modules can have one of the two priviledge level.
//! A priviledged module can access to the server core datas.
//! Modules have to register hooks being called at different stages
//! of the request processing flow.
//! See the API documentation for more information on how
//! a request is processed by the server core.


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

//! \fn bool server::modman::load_at_end(const std::string& id,
//!					 bool priviledged = false,
//!					 bool activ = true)
//! \brief load a module at the end of the list.
//!
//! \param id Path identifying the module
//! \param priviledged wether or not the module is a priviledged one
//! \param activ is the module activated at loading
//! \return false on error (either the module is not found, permission denied...).
//!
//! Load a module at the end of the modlist_.

//! \fn bool server::modman::load(const std::string& after_id,
//!				  const std::string& my_id,
//!				  const std::string& before_id,
//!				  bool priviledged = false,
//!				  bool activ = true,
//!				  bool load_missing = true)
//! \brief load a module between two others.
//!
//! \param after_id path identifying the module to load after
//! \param my_id path identifying the module to load
//! \param before_id path identifying the module to load before
//! \param priviledged wether or not the module is a priviledged one
//! \param activ is the module activated at loading
//! \param load_missing load the missing module
//! \return false on error (either the module is not found, permission denied...).
//!
//! Load the module identified by my_id AFTER after_id, and BEFORE before_id.
//! If the one or all module doesn't exist, the boolean load_missing decides wether
//! or not to load them.


//! \fn bool server::modman::reload(const std::string& id,
//!		    bool priviledged = false,
//!		    bool activ = true)
//! \brief reload a module if it exists, preserving the position in the list.
//!
//! \param id Path identifying the module
//! \param priviledged wether or not the module is a priviledged one
//! \param activ is the module activated at loading
//! \return false if the module isnot present, or cannot be accessed.
//!
//! Reload a module at the same place in modlist_.


//! \fn bool server::modman::is_loaded(const std::string& id) const
//! \brief Tell if the module is present in the list.
//!
//! \param id Path identifying the module
//! \return false if the module isnot present
//!
//! Tell if the module is present in the list.


//! \fn bool server::modman::unload(const std::string& id)
//! \brief Unload an existing module
//!
//! \param id Path identifying the module
//! \return false if the module isnot present.
//!
//! Unload the module identified by id in modlist_.

//! \fn bool server::modman::start(const std::string& id)
//! \brief Start the module.
//!
//! \param id Path identifying the module
//! \return false if the module isnot present or already running.
//!
//! Start the module.

//! \fn bool server::modman::stop(const std::string& id)
//! \brief Stop the module.
//!
//! \param id Path identifying the module
//! \return false if the module isnot present or not running.
//!
//! Stop the module.

//! \fn bool server::modman::state(const std::string& id, int& st)
//! \brief Tell why the module is in state...
//!
//! \param id Path identifying the module
//! \param st Code of the module state
//! \return false if the module isnot present.
//!
//! Tell why the module is in state...

//! \fn bool server::modman::call_hooks(server::core* core, stageid id, http::session* session)
//! \brief Call hooks registered by modules for a given stage
//!
//! \param core Address of the core to be passed to priviledged modules
//! \param id Processing request flow stage identifier
//! \param session Current session being processed by the server
//! \return Always return true(improve it).
//!
//! Call hooks registered by modules for a given stage in
//! the request processing flow.
//! \see module.hh for informations about module hooks.

//! \fn server::module& server::modman::operator[](const std::string& id);
//! \brief Get the module identified by id.
//!
//! \param id Path identifying the module
//! \return A reference to the module pointer contained in modlist_
//!
//! Get the module identified by id.


#endif // ! SERVER_MODMAN_HH
