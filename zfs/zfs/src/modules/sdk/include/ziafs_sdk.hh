//
// ziafs_sdk.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 00:01:22 2006 texane
// Last update Thu Jan 26 02:59:19 2006 texane
//


#ifndef ZIAFS_SDK_HH
# define ZIAFS_SDK_HH


// ?
// Include this file when
// developping your own modules


// Forward declarations
namespace net { class module; }
namespace net { class session; }


// Export a function with this macro
#if defined (_WIN32)
# define MOD_EXPORT(f)		extern "C" __declspec(dllexport) status::error f
#else
# define MOD_EXPORT(f)		extern "C" status::error f
#endif // _WIN32

# define HK_STRFY( s )		#s
# define HK_SYM( sym )		HK_STRFY( sym )



#endif // ! ZIAFS_SDK_HH
