//
// resource_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:53:14 2005 texane
// Last update Wed Nov 23 14:30:47 2005 texane
//


#include <string>
#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>


using std::string;
using dataman::buffer;
using dataman::resource::error_t;


dataman::file::file(const string& filename)
{
}


bool	dataman::file::open(error_t&)
{
  return false;
}


bool	dataman::file::fetch(buffer&, unsigned int, error_t&)
{
  return false;
}


bool	dataman::file::fetch(buffer&, error_t&)
{
  return false;
}


bool	dataman::file::close(error_t&)
{
  return false;
}
