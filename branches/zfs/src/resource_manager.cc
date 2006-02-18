//
// resource_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:04:04 2006 texane
// Last update Sat Feb 18 01:46:21 2006 texane
//


#include <string>
#include <ziafs_resource.hh>


using std::string;


resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
						    resource::e_id res_id,
						    resource::e_omode res_omode,
						    unsigned int st_code)
{
  resource::e_error err;

  // instanciate the resource
  err = E_SUCCESS;
  res_handle = 0;
  switch (res_id)
    {
    case ID_BYFLY:
      res_handle = new byfly(st_code);
      break;
    default:
      err = E_UNKNOWN;
      break;
    }

  // set the infos
  res_handle->omode = res_omode;
  res_handle->id = res_id;
  return err;
}


resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
						    resource::e_id res_id,
						    resource::e_omode res_omode,
						    const string& res_name)
{
  resource::e_error err;

  // instanciate the resource
  err = E_SUCCESS;
  res_handle = 0;
  switch (res_id)
    {
    case ID_FILE:
      res_handle = new file(res_name, res_omode);
      break;
    default:
      err = E_UNKNOWN;
      break;
    }

  // set the infos
  res_handle->omode = res_omode;
  res_handle->id = res_id;
  return err;
}


resource::e_error resource::manager::factory_destroy(resource::handle* res_handle)
{
  delete res_handle;
  return E_SUCCESS;
}
