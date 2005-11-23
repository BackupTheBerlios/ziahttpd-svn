//
// resource_report.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:53:31 2005 texane
// Last update Wed Nov 23 16:10:34 2005 texane
//


#include <string>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>


using std::string;
using dataman::buffer;


dataman::report::report(unsigned int stcode)
{
}


bool	dataman::report::open(error_t&)
{
  return false;
}


bool	dataman::report::fetch(buffer&, unsigned int, error_t&)
{
  return false;
}


bool	dataman::report::fetch(buffer&, error_t&)
{
  return false;
}


bool	dataman::report::close(error_t&)
{
  return false;
}
