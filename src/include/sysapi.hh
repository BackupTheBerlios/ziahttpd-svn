//
// sysapi.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 09 13:56:26 2005 texane
// Last update Wed Oct 19 23:05:00 2005 
//

#ifndef SYSAPI_HH
# define SYSAPI_HH


// Address the portability issue by
// namespace aliasing means

#if _SYSAPI == 1
# include <posix.hh>
namespace sysapi = posix;

#elif _SYSAPI == 2
# include <win32.hh>
namespace sysapi = win32;

#else
# error SYSAPI NOT UNDERSTOOD
#endif // ! _SYSAPI



// - Doxygen comments start from here

//! \file
//! \brief Implements a software layer above win32 and posix apis.
//!
//! Interfaces available for file, process, thread.
//! See the files win32.hh and posix.hh for more informations.


#endif // ! SYSAPI_HH
