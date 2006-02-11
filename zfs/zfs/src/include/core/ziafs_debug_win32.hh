//
// ziafs_debug_win32.hh for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:44:01 2006 
// Last update Sat Feb 11 17:44:42 2006 
//


#ifndef ZIAFS_DEBUG_WIN32_HH
# define ZIAFS_DEBUG_WIN32_HH

# include <cstdio>
# define ziafs_debug_msg( fmt, __VA_ARGS__ ) \
do \
{ \
  ::printf("(@%s#%d): " fmt, __FILE__ , __LINE__ , __VA_ARGS__); \
  ::fflush(stdout); \
} \
while (0)
#else
# define ziafs_debug_msg( fmt, __VA_ARGS__ ) \
do {} while(0)

#endif // ! ZIAFS_DEBUG_WIN32_HH
