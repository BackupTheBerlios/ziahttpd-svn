//
// resource.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:58:22 2005 texane
// Last update Thu Nov 24 16:56:15 2005 texane
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
