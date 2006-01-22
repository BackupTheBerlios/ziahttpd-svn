//
// common_api.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 14:07:22 2006 texane
// Last update Sun Jan 22 16:01:49 2006 texane
//


#ifndef SYS_COMMON_API_HH
# define SYS_COMMON_API_HH


#include <string>

namespace sys
{
#ifdef _WIN32
  namespace win32_api
#else
  namespace posix_api
#endif // _WIN32
  {
    // Error related part
    namespace error
    {
      typedef enum
	{
	  // not an error
	  SUCCESS = 0,

	  // file operations

	  // process operations

	  // thread operations
	
	  // inSocket operations

	  // unknown error
	  UNKNOWN
	} errval_t;

      std::string tostring(const handle_t&);
      bool is_success(const handle_t&);
      bool is_failure(const handle_t&);
      errval_t tovalue(const handle_t&);
    }

    // file related part
    namespace file
    {
      error::handle_t open(handle_t&, const std::string&);
      error::handle_t close(handle_t&);
    }
  }

}


#endif // ! SYS_COMMON_API_HH
