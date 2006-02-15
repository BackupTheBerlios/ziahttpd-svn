//
// ziafs.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 16:03:48 2006 texane
// Last update Wed Feb 15 22:38:54 2006 
//


#ifndef ZIAFS_HH
# define ZIAFS_HH


#include <ziafs_thr.hh>
#include <ziafs_net.hh>
#include <ziafs_core.hh>
#include <ziafs_config.hh>
#include <ziafs_debug.hh>
#include <ziafs_static.hh>
#include <sys/sysapi.hh>

#ifndef _WIN32
# include <unistd.h>
# define Sleep( n ) usleep(n)
#endif // ! _WIN32

#ifdef _WIN32
# include <sys/pthread.h>
# include <windows.h>
#else
# include <pthread.h>
#endif // _WIN32


#endif // ! ZIAFS_HH
