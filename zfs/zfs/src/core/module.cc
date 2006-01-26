//
// module.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Jan 26 00:39:17 2006 texane
// Last update Thu Jan 26 01:09:01 2006 texane
//


#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_mod.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_status.hh>


using std::string;


net::module::~module()
{
  sysapi::module::unload(m_hmod);
}
