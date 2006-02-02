//
// sysapi.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 13:56:18 2006 texane
// Last update Thu Feb 02 14:36:22 2006 texane
//

#ifndef SYS_API_HH
# define SYS_API_HH


#ifdef _WIN32
#ifndef FD_SETSIZE
# define FD_SETSIZE 4096
#endif
# include <winsock2.h>
# include <windows.h>
# include <sys/win32_api.hh>
# include <sys/common_api.hh>
namespace sysapi = sys::win32_api;
#else
# include <sys/posix_api.hh>
# include <sys/common_api.hh>
namespace sysapi = sys::posix_api;
#endif



#endif // ! SYS_API_HH
