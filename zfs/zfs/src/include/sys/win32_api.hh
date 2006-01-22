//
// win32_api.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 13:59:25 2006 texane
// Last update Sun Jan 22 15:58:11 2006 texane
//


#ifndef SYS_WIN32_API_HH
# define SYS_WIN32_API_HH


#include <windows.h>


namespace sys
{
  namespace win32_api
  {
    namespace error
    {
      // Error related
      typedef DWORD handle_t;
    }

    namespace file
    {
      // File related part
      typedef HANDLE handle_t;
    }
  }
}


#endif // ! SYS_WIN32_API_HH
