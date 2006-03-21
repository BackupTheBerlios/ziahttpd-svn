//
// ziafs_mod.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 15:10:34 2006 texane
// Last update Tue Mar 21 16:31:17 2006 texane
//


#ifndef ZIAFS_MOD_HH
# define ZIAFS_MOD_HH


#include <list>
#include <string>
#include <sys/sysapi.hh>
#include <ziis.hh>


namespace net { class config; }
namespace mod { class pipeline; }

namespace mod
{
  class modinfo
  {
    friend class pipeline;

  public:
    modinfo(const std::string&);
    ~modinfo();

  private:
    sysapi::module::handle_t m_h_module;
    IModule* m_id_module;
    std::string m_nm_module;
  };
}


namespace mod
{
  class pipeline
  {
  public:
    pipeline();
    ~pipeline();
    
    bool rebuild(net::config*);
    bool load_module(const std::string&);
    bool unload_module(const std::string&);
    bool execute();
    bool find(const std::string&);


  private:

    std::list<modinfo*> m_modules;
    
    void release();
    void reset();
  };
}


#endif // ! ZIAFS_MOD_HH
