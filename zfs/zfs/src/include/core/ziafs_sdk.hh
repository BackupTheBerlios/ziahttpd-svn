//
// ziafs_sdk.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 00:01:22 2006 texane
// Last update Sun Jan 22 18:17:08 2006 texane
//


#ifndef ZIAFS_SDK_HH
# define ZIAFS_SDK_HH


// ?
// Include this file when
// developping your own modules


// Export a function with this macro
#if defined (_WIN32)
# define MOD_EXPORT(f)		extern "C" __declspec(dllexport) bool f
#else
# define MOD_EXPORT(f)		extern "C" bool f
#endif // _WIN32

# define HK_STRFY( s )		#s
# define HK_SYM( sym )		HK_STRFY( sym )


namespace sdk
{
}


#endif // ! ZIAFS_SDK_HH
