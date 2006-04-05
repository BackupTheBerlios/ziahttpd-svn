//
// ziafs_mod.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 15:10:34 2006 texane
// Last update Wed Apr 05 14:59:38 2006 texane
//


#ifndef ZIAFS_MOD_HH
# define ZIAFS_MOD_HH


#include <list>
#include <string>
#include <vector>
#include <sys/sysapi.hh>
#include <ziis.hh>


namespace net { class config; }
namespace mod { class manager; }
class IFS;


namespace mod
{
  class modinfo
  {
    friend class manager;
  public:
    modinfo(const std::string&);
    ~modinfo();

    bool reload();

  private:
    sysapi::module::handle_t m_handle;
    IModule* m_instance;
    std::string m_path;
  };
}

namespace mod
{
  class manager
  {
  public:
    manager();
    ~manager();

    bool reload(net::config*, IFS* = 0);
    bool get_connection_module(IConnection*&);
    bool get_compressor_module(ICompressor*&, std::vector<std::string>&, std::string&);
    bool get_generator_module(IDocumentGenerator*&, const std::string&);
    bool get_modifier_list(std::list<IStreamModifier*>&, const std::string&);

  private:
    std::list<modinfo*> m_modlist;

    // internal management
    bool load_module(const std::string&, const std::string&, IFS* = 0);
  };
}


#endif // ! ZIAFS_MOD_HH
