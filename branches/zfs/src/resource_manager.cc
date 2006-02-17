//
// resource_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:04:04 2006 texane
// Last update Fri Feb 17 13:49:33 2006 texane
//


#include <string>
#include <ziafs_resource.hh>


using std::string;


resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
						    resource::e_id res_id,
						    resource::e_omode res_omode,
						    const string& res_name)
{
  res_handle = 0;
  // res_handle = new...
  return E_NOT_IMPL;
}


resource::e_error resource::manager::factory_destroy(resource::handle* res_handle)
{
//   delete res_handle;
  return E_NOT_IMPL;
}
