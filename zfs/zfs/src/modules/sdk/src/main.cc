//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Jan 26 01:54:17 2006 texane
// Last update Thu Jan 26 03:00:18 2006 texane
//


#include <string>
#include <iostream>
#include <ziafs_sdk.hh>
#include <core/ziafs_mod.hh>
#include <core/ziafs_status.hh>

// Simple module for the ziafs server


using std::string;


namespace net
{
  class mod_sdk : public module
  {
  public:
    ~mod_sdk();
    virtual status::error accept_connection(session*, const string&, unsigned short);
    virtual status::error register_filters(session*);
  };
}


// interface implementation

status::error net::mod_sdk::accept_connection(session* session, const string& addr, unsigned short port)
{
  std::cout << "in the connection module!..." << std::endl;
  ziafs_return_status( status::NOTIMPL );
}


status::error net::mod_sdk::register_filters(session*)
{
  ziafs_return_status( status::NOTIMPL );
}


net::mod_sdk::~mod_sdk()
{
  std::cout << "in the module destructor!..." << std::endl;
}


// @ module loading

MOD_EXPORT(mod_entry)(net::module*&);

MOD_EXPORT(mod_entry)(net::module*& module)
{
  module = new net::mod_sdk;
  ziafs_return_status( status::SUCCESS );
}
