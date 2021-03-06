//
// resource_manager.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 10:37:39 2006 texane
// Last update Sun Apr 02 21:03:50 2006 texane
//


#include <string>
#include <iostream>
#include "include/buffer.hh"
#include "include/resource.hh"


using std::string;


// process resource
resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
						    resource::e_id res_id,
						    resource::e_omode res_omode,
						    int ac, char** av, char** env,
						    unsigned int res_insz)
{
  resource::e_error e_err;

  // instanciate the resource
  e_err = E_SUCCESS;
  res_handle = 0;
  res_handle = new process(ac, av, env, res_omode);

  // set informations
  res_handle->omode = res_omode;
  res_handle->id = res_id;
  res_handle->in_size = res_insz;
  return e_err;
}


// byfly resource
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
  res_handle->in_size = 0;
  return err;
}


// file resource
resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
						    resource::e_id res_id,
						    resource::e_omode res_omode,
						    const string& res_name,
						    unsigned int res_insz)
{
  resource::e_error err;

  // instanciate the resource
  err = E_SUCCESS;
  res_handle = 0;
  res_handle = new file(res_name, res_omode);

  // set the infos
  res_handle->omode = res_omode;
  res_handle->id = res_id;
  res_handle->in_size = 0;
  if (res_omode == O_OUTPUT || res_omode == O_BOTH)
    res_handle->in_size = res_insz;
  return err;
}


// fake resource
resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
						    resource::e_id res_id)
{
  res_handle = new fake();
  res_handle->omode = O_BOTH;
  res_handle->id = res_id;
  res_handle->in_size = 0;
  return E_SUCCESS;
}


resource::e_error resource::manager::factory_destroy(resource::handle* res_handle)
{
  if (res_handle)
    delete res_handle;
  return E_SUCCESS;
}
