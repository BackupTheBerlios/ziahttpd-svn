//
// sysapi.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 09 13:56:26 2005 texane
// Last update Tue Oct 11 14:52:39 2005 texane
//

#ifndef SYSAPI_HH
# define SYSAPI_HH

// Address the subsystem portability issue
// at the compile time, by aliasing the sysapi
// namespace to the correct one.
// _SYSAPI has to be defined one the command line,
// and must be affected with the following values:
// {win32, posix}.

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
