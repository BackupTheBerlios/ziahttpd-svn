//
// resource.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:58:22 2005 texane
// Last update Sun Dec 04 17:04:35 2005 texane
//


#include <string>
#include <vector>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>


using std::string;
using std::vector;
using dataman::buffer;
using dataman::resource;


// -
// This file implements resource
// factory by the factory method
// design pattern


#include <iostream>
#include <sysapi/sysapi.hh>


resource* dataman::resource::factory(const string& filename)
{
  return new file(filename);
}


resource* dataman::resource::factory(const std::vector<const std::string>& av,
				     const std::vector<const std::string>& env)
{
  return new cgi(av, env);
}


resource* dataman::resource::factory(unsigned int status_code)
{
  return new report(status_code);
}


resource* dataman::resource::factory(sysapi::socket_in::handle_t hsock,
				     sysapi::socket_in::size_t szbody,
				     bool chunked)
{
  if (chunked == true)
    { return 0; }
  return new bodydata(hsock, szbody);
}
