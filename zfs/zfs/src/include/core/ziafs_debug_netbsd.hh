//
// ziafs_debug_netbsd.hh for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:43:07 2006 
// Last update Sat Feb 11 17:45:12 2006 
//


#ifndef ZIAFS_DEBUG_NETBSD_HH
# define ZIAFS_DEBUG_NETBSD_HH

#ifdef ZIAFS_DEBUG_MSG
# include <cstdio>
# define ziafs_debug_msg( fmt, args... ) \
do \
{ \
  ::printf("(@%s#%d): " fmt, __FILE__ , __LINE__ , ##args); \
  ::fflush(stdout); \
} \
while (0)
#else
# define ziafs_debug_msg( fmt, args... ) \
do {} while(0)
#endif // ZIAFS_DEBUG_MSG


#endif // ! ZIAFS_DEBUG_NETBSD_HH
