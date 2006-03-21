//
// ziis_impl.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 18:49:56 2006 texane
// Last update Tue Mar 21 19:06:17 2006 texane
//


#ifndef ZIIS_IMPL_HH
# define ZIIS_IMPL_HH


#include <ziafs_http.hh>
#include <sys/sysapi.hh>
#include <ziis.hh>


class ZfsInput : public IInput
{
public:
  ZfsInput(net::http&) { }
  ~ZfsInput() {}

  const char* GetInput(const char*){ return 0; }
  const char* GetInputMethod(){ return 0; }
  const char* GetInputLocation(){ return 0; }
  const char* GetInputHttpVersion(){ return 0; }
  const char* GetInputQueryString(){ return 0; }
  int ReadPostEntity(char*, int){ return -1;}
  bool GetNextHeader(char**, char**){ return false; }
};


#endif // ! ZIIS_IMPL_HH
