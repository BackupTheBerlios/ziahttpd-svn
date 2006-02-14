//
// win32_api.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 13:59:25 2006 texane
// Last update Sat Feb 11 17:50:43 2006 
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
  }
}


#endif // ! SYS_WIN32_API_HH
