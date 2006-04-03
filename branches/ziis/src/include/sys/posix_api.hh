//
// posix_api.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 14:01:21 2006 texane
// Last update Mon Apr  3 15:15:09 2006 
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

    namespace mmapping
    {
      typedef struct
      {
	int id_file;
	int id_mapping;
	unsigned long long sz_mmapping;
	unsigned char* p_mmapping;
      } handle_t;
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

    namespace process
    {
      typedef pid_t handle_t;
    }
  }
}


#endif // ! SYS_POSIX_API_HH
