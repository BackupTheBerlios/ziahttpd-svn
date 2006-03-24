//
// win32_api.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 13:59:25 2006 texane
// Last update Fri Mar 24 18:44:57 2006 texane
//


#ifndef SYS_WIN32_API_HH
# define SYS_WIN32_API_HH


namespace sys
{
  namespace win32_api
  {
    namespace file
    {
      // File related part
      typedef HANDLE handle_t;
    }

    namespace mmapping
    {
      typedef struct
      {
	HANDLE h_file;
	HANDLE h_mapping;
	unsigned long long sz_mmapping;
	unsigned char* p_mmapping;
      } handle_t;
    }

    namespace insock
    {
      // internet socket part
      typedef SOCKET handle_t;
    };

    namespace module
    {
      // module loading, unloading, resolving
      typedef HMODULE handle_t;
    }

    namespace time
    {
      typedef unsigned long long handle_t;
    }

    namespace process
    {
      typedef HANDLE handle_t;
    }
  }
}


#endif // ! SYS_WIN32_API_HH
