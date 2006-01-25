//
// iores_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 19:11:31 2006 texane
// Last update Wed Jan 25 19:34:35 2006 texane
//


#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_status.hh>


using std::string;
using namespace status;


io::res_manager::res_manager()
{}


io::res_manager::~res_manager()
{}


status::error io::res_manager::create(resource*&, stmask, const std::string&, unsigned short)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::create(resource*&, stmask, const struct sockaddr_in&, const sysapi::insock::handle_t&)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::create(resource*&, stmask, const std::string&)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::destruct(resource*)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::open(resource*)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::close(resource*)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::fetch(resource*, void*&)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::feed(resource*, void*&)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::process_all()
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::load_config(const net::config&)
{
  ziafs_return_status( NOTIMPL );
}
