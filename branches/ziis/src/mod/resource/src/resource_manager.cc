//
// resource_manager.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 10:37:39 2006 texane
// Last update Wed Mar 22 11:18:37 2006 texane
//


#include <string>
#include <iostream>
#include "include/buffer.hh"
#include "include/resource.hh"


using std::string;


// process resource
// resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
// 						    resource::e_id res_id,
// 						    resource::e_omode res_omode,
// 						    int ac, char** av, char** env,
// 						    buffer* res_inbuf,
// 						    unsigned int res_insz)
// {
//   resource::e_error e_err;
//   unsigned int to_remove;

//   // instanciate the resource
//   e_err = E_SUCCESS;
//   res_handle = 0;
//   res_handle = new process(ac, av, env, res_omode);

//   // set informations
//   res_handle->omode = res_omode;
//   res_handle->id = res_id;
//   res_handle->in_size = res_insz;
//   if (res_inbuf)
//     {
//       to_remove = res_insz;
//       if (to_remove > res_inbuf->size())
// 	to_remove = (unsigned int)res_inbuf->size();
//       res_handle->in_buf = buffer(res_inbuf->bufptr(), to_remove);
//       res_inbuf->remove_front(to_remove);
//     }
//   return e_err;
// }


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
						    buffer* res_inbuf,
						    unsigned int res_insz)
{
  resource::e_error err;
  unsigned int to_remove;

  // instanciate the resource
  err = E_SUCCESS;
  res_handle = 0;
  res_handle = new file(res_name, res_omode);

  // set the infos
  res_handle->omode = res_omode;
  res_handle->id = res_id;
  res_handle->in_size = 0;
  if (res_omode == O_OUTPUT || res_omode == O_BOTH)
    {
      res_handle->in_size = res_insz;
      if (res_inbuf)
	{
	  to_remove = res_insz;
	  if (to_remove > res_inbuf->size())
	    to_remove = (unsigned int)res_inbuf->size();
	  res_handle->in_buf = buffer(res_inbuf->bufptr(), to_remove);
	  res_inbuf->remove_front(to_remove);
	}
    }
  return err;
}


// fake resource
// resource::e_error resource::manager::factory_create(resource::handle*& res_handle,
// 						    resource::e_id res_id)
// {
//   res_handle = new fake();
//   res_handle->omode = O_BOTH;
//   res_handle->id = res_id;
//   res_handle->in_size = 0;
//   return E_SUCCESS;
// }


resource::e_error resource::manager::factory_destroy(resource::handle* res_handle)
{
  delete res_handle;
  return E_SUCCESS;
}
