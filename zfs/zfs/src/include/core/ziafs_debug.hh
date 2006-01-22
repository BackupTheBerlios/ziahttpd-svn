//
// ziafs_debug.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 17:50:09 2006 texane
// Last update Sun Jan 22 18:02:27 2006 texane
//


#ifndef ZIAFS_DEBUG_HH
# define ZIAFS_DEBUG_HH


#ifdef ZIAFS_DEBUG_MSG
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
#endif // ZIAFS_DEBUG_MSG


#endif // ! ZIAFS_DEBUG_HH
