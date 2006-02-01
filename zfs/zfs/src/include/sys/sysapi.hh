//
// sysapi.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 13:56:18 2006 texane
// Last update Wed Feb 01 01:16:44 2006 texane
//

#ifndef SYS_API_HH
# define SYS_API_HH


#ifdef _WIN32
# include <sys/win32_api.hh>
# include <sys/common_api.hh>
namespace sysapi = sys::win32_api;
#else
# include <sys/posix_api.hh>
# include <sys/common_api.hh>
namespace sysapi = sys::posix_api;
#endif



#endif // ! SYS_API_HH
