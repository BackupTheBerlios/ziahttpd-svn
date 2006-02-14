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


#ifdef _WIN32
# include <sys/pthread.h>
#else
# include <pthread.h>
#endif // _WIN32


#ifdef _WIN32
# include <ziafs_debug_win32.hh>
#else
# include <ziafs_debug_netbsd.hh>
#endif // _WIN32


#endif // ! ZIAFS_DEBUG_HH
