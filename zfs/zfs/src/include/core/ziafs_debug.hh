//
// ziafs_debug.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 17:50:09 2006 texane
// Last update Sat Feb 11 17:45:59 2006 
//


#ifndef ZIAFS_DEBUG_HH
# define ZIAFS_DEBUG_HH


#ifdef __NetBSD__
# include <core/ziafs_debug_netbsd.hh>
#else
# include <core/ziafs_debug_win32.hh>
#endif // __NetBSD__


#endif // ! ZIAFS_DEBUG_HH
