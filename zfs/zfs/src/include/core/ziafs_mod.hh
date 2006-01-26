//
// ziafs_mod.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 23:26:09 2006 texane
// Last update Thu Jan 26 03:01:23 2006 texane
//



#ifndef ZIAFS_MOD_HH
# define ZIAFS_MOD_HH


#include <list>
#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_status.hh>


// Every time a module is loaded,
// it can new() itself in order to
// fit the following interface.
// Registering function should be
// called mod_init();

namespace net
{
  class mod_manager;
  class session;

  class module
  {
    friend class mod_manager;

  public:
    // Module construction/destruction
    virtual ~module()
    {
      sysapi::module::unload(m_hmod);
    }

    // This method register a new resource
    // that will bound a socket listening for
    // incoming connections.
    virtual status::error accept_connection(session*, const std::string&, unsigned short) = 0;
    virtual status::error register_filters(session*) = 0;

  private:
    std::string m_name;
    sysapi::module::handle_t m_hmod;
  };
}


namespace net
{
  class config;

  class mod_manager
  {
  public:
    mod_manager(config*);
    ~mod_manager();
    status::error load_module(const std::string&);
    status::error unload_module(const std::string&);
    status::error find_module(module*&, const std::string&);

  private:
    std::list<module*> m_modules;
  };
}


#endif // ! ZIAFS_MOD_HH



// load_all_modules(conf);
// foreach port
// {
//   mod = get_module_byname(name_of_module);

//   // register resource client
//   // add to resource manager
//   mod->accept_connection(session, port);
// }


// session_process()
// {
//   session->proto->process_stage_fn(this);
// }

// first:
// if (session->client->fetch(buf))
//  meda_done = session->proto->consum(buf);

// second:
// proto->process_metadata();

// third:
// foreach module
// {
//   mod->register_filters(session);
// }

// end:
// if (session->res_client->fetch(buf))
//   sssion->res_target->feed(buf);
// if (session->res_target->fetch(buf))
//   session->res_client->feed(buf);
