//
// posix.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 09 13:57:14 2005 texane
// Last update Tue Oct 11 15:05:44 2005 texane
//


#ifndef SYSAPI_POSIX_HH
# define SYSAPI_POSIX_HH

// @see sysapi.hh
// In order to handle non portable fashions
// among posix compliant systems, you should
// Use #ifdef compile time statements instead of
// creating specific namespaces (such as posix_sunos for instance)

// @ not yet done
// #include <posix_file.hh>
// #include <posix_process.hh>
// #include <posix_thread.hh>

namespace posix
{
  namespace file {}
  namespace process {}
  namespace thread {}
  namespace error {}
}



// - Doxygen comments start from here

//! \file
//! \brief Implements the win32 part of sysapi
//!
//! Interfaces available for file, process, thread.
//! See the files win32.hh for more informations.


#endif // ! SYSAPI_POSIX_HH
