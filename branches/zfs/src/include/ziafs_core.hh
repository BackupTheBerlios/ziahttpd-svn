//
// ziafs_core.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 16:42:56 2006 texane
// Last update Tue Feb 14 17:27:32 2006 texane
//


#ifndef ZIAFS_CORE_HH
# define ZIAFS_CORE_HH


// The core is glue that
// tights all together.


#include <list>


// Forward declarations
namespace thr { class pool; }
namespace net { class server; }

// Core
typedef struct core
{
  thr::pool* thr_pool;
  std::list<net::server*> srv_list;
} core_t;


#endif // ! ZIAFS_CORE_HH
