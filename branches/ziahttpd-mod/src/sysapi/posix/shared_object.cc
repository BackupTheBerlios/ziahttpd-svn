//
// shared_object.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:14:17 2005 
// Last update Sun Nov 13 21:50:26 2005 
//


#include <string>
#include <dlfcn.h>
#include <sysapi/posix.hh>
#include <iostream>


using std::string;
using std::cout;
using std::cerr;
using std::endl;


bool	posix::shared_object::open(handle_t& hobj, const string& name, error_t&)
{
#if defined (linux)
# define DL_LAZY RTLD_LAZY
#endif // __linux__

  if ((hobj = dlopen(reinterpret_cast<const char*>(name.c_str()), DL_LAZY)) == NULL)
    return false;
  return true;
}


bool	posix::shared_object::close(handle_t& hobj)
{
  dlclose(hobj);
  return true;
}


bool	posix::shared_object::resolve(void*& symaddr, handle_t& hobj, const string& name, error_t&)
{
  if ((symaddr = dlsym(hobj, reinterpret_cast<const char*>(name.c_str()))) == NULL)
    return false;
  return true;
}
