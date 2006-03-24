//
// mmapping.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Mar 24 18:24:17 2006 texane
// Last update Fri Mar 24 18:24:25 2006 texane
//


#include <string>
#include <sys/sysapi.hh>


using std::string;


sysapi::error::handle_t sysapi::mmapping::alloc(handle_t&, const string& nm_file)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::mmapping::release(handle_t&)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::mmapping::reset(handle_t&)
{
  return error::SUCCESS;  
}


sysapi::error::handle_t sysapi::mmapping::size(handle_t&, unsigned long long& sz)
{
  return error::SUCCESS;  
}
