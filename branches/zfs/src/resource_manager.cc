//
// resource_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 13:04:04 2006 texane
// Last update Sat Feb 18 11:50:58 2006 texane
//


#include <string>
#include <ziafs_resource.hh>


using std::string;


resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
						    resource::e_id res_id,
						    resource::e_omode res_omode,
						    int ac, char** av, char** env)
{
  resource::e_error e_err;

  // instanciate the resource
  e_err = E_SUCCESS;
  res_handle = 0;
  res_handle = new process(ac, av, env, res_omode);

  // set informations
  res_handle->omode = res_omode;
  res_handle->id = res_id;
  return e_err;
}


resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
						    resource::e_id res_id,
						    resource::e_omode res_omode,
						    unsigned int st_code)
{
  resource::e_error err;

  // instanciate the resource
  err = E_SUCCESS;
  res_handle = 0;
  res_handle = new byfly(st_code);

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
  res_handle = new file(res_name, res_omode);

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
