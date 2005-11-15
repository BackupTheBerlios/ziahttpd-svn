//
// sysapi.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 13:54:54 2005 
// Last update Sun Nov 13 13:57:34 2005 
//


#ifndef SYSAPI_HH
# define SYSAPI_HH


// Address the portability issue by
// namespace aliasing means

#if _SYSAPI == 1
# include <sysapi/posix.hh>
namespace sysapi = posix;

#elif _SYSAPI == 2
# include <sysapi/win32.hh>
# undef interface
namespace sysapi = win32;

#else
# error SYSAPI NOT UNDERSTOOD
#endif // ! _SYSAPI




//! \file
//! \brief Implements a software layer above win32 and posix apis.
//!
//! Interfaces available for file, process, thread.
//! See the files win32.hh and posix.hh for more informations.



#endif // ! SYSAPI_HH
