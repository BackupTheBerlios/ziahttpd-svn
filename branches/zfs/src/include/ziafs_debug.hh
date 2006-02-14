//
// ziafs_debug.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 17:50:09 2006 texane
// Last update Tue Feb 14 11:19:15 2006 texane
//


#ifndef ZIAFS_DEBUG_HH
# define ZIAFS_DEBUG_HH


#include <pthread.h>


#ifdef __NetBSD__
# include <ziafs_debug_netbsd.hh>
#else
# include <ziafs_debug_win32.hh>
#endif // __NetBSD__


#endif // ! ZIAFS_DEBUG_HH
