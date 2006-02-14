//
// posix_api.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 14:01:21 2006 texane
// Last update Sat Feb 11 17:50:20 2006 
//


#ifndef SYS_POSIX_API_HH
# define SYS_POSIX_API_HH

namespace sys
{
  namespace posix_api
  {
    namespace file
    {
      typedef int handle_t;
    }

    namespace insock
    {
      typedef int handle_t;
    }

    namespace module
    {
      typedef void* handle_t;
    }

    namespace time
    {
      typedef unsigned long long handle_t;
    }

  }
}


#endif // ! SYS_POSIX_API_HH
