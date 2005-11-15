//
// module.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 22:45:49 2005 
// Last update Mon Nov 14 13:21:56 2005 
//


#include <string>
#include <iostream>
#include <sysapi/sysapi.hh>
#include <http/session.hh>
#include <server/module.hh>
#include <server/modhelper.hh>


using std::string;
using std::cerr;
using std::cout;
using std::endl;


server::module::module(const string& name, bool priviledged, bool active)
{
  sysapi::shared_object::error_t err;

  reset();

  priviledged_ = priviledged;
  running_ = active;

  if (sysapi::shared_object::open(hobj_, name, err) == false)
    {
      sysapi::error::stringify("CANNOT LOAD MODULE");
    }
  else
    {
      // Resolve hooks to be called at different stages,
      // no need to check return values since hooks are
      // defaulted to null.
      sysapi::shared_object::resolve((void*&)hk_create_con_, hobj_,		HK_SYM( HK_CREATE_CONNECTION ), err);
      sysapi::shared_object::resolve((void*&)hk_get_rqstmetadata_, hobj_,	HK_SYM( HK_GET_RQST_METADATA ), err);
      sysapi::shared_object::resolve((void*&)hk_get_rqstdata_, hobj_,		HK_SYM( HK_GET_RQST_DATA ), err);
      sysapi::shared_object::resolve((void*&)hk_parse_rqstmetadata_, hobj_,	HK_SYM( HK_PARSE_RQST_METADATA ), err);
      sysapi::shared_object::resolve((void*&)hk_alter_rqstdata_, hobj_,		HK_SYM( HK_ALTER_RQST_DATA ), err);
      sysapi::shared_object::resolve((void*&)hk_build_respmetadata_, hobj_,	HK_SYM( HK_BUILD_RESP_METADATA ), err);
      sysapi::shared_object::resolve((void*&)hk_build_respdata_, hobj_,		HK_SYM( HK_BUILD_RESP_DATA ), err);
      sysapi::shared_object::resolve((void*&)hk_alter_respdata_, hobj_,		HK_SYM( HK_ALTER_RESP_DATA ), err);
      sysapi::shared_object::resolve((void*&)hk_alter_respmetadata_, hobj_,	HK_SYM( HK_ALTER_RESP_METADATA ), err);
      sysapi::shared_object::resolve((void*&)hk_send_response_, hobj_,		HK_SYM( HK_SEND_RESPONSE ), err);
      sysapi::shared_object::resolve((void*&)hk_release_con_, hobj_,		HK_SYM( HK_RELEASE_CONNECTION ), err);
    }
}


server::module::~module()
{
  sysapi::shared_object::close(hobj_);
}


bool	server::module::reset()
{
  // Default hooks to null
  hk_create_con_ = 0;
  hk_get_rqstmetadata_ = 0;
  hk_get_rqstdata_ = 0;
  hk_parse_rqstmetadata_ = 0;
  hk_alter_rqstdata_ = 0;
  hk_build_respmetadata_ = 0;
  hk_build_respdata_ = 0;
  hk_alter_respdata_ = 0;
  hk_alter_respmetadata_ = 0;
  hk_send_response_ = 0;
  hk_release_con_ = 0;

  priviledged_ = false;
  running_ = false;

  return true;
}
