//
// ziafs_status.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:22:54 2006 texane
// Last update Sun Jan 22 16:27:28 2006 texane
//


#ifndef ZIAFS_STATUS_HH
# define ZIAFS_STATUS_HH


#include <string>


namespace status
{

#ifdef ZIAFS_DEBUG_ERROR
  enum { debug_err = 1 };
#else
  enum { debug_err = 0 };
#endif // ZIAFS_DEBUG_ERROR

#ifdef ZIAFS_LOG_ERROR
  enum { log_err = 1 };
#else
  enum { log_err = 0 };
#endif // ZIAFS_LOG_ERROR

  typedef enum
    {
      SUCCESS = 0,
      NOTIMPL,
      BADMODE,
      UNKOWN
    } error;

  void debug_err_fn(error, const char* filename = "", int nline = 0);
  void log_err_fn(error, const char* filename = "", int nline = 0);
  std::string dump(error, const char* filename = "", int nline = 0);
  std::string tostring(error);
}


// return on error
#define ziafs_return_status( s )		\
{						\
  if (status::debug_err == 1)			\
    status::debug_err_fn(s, __FILE__, __LINE__);\
  if (status::log_err == 1)			\
    status::log_err_fn(s, __FILE__, __LINE__);	\
  return s;					\
}


#endif // ! ZIAFS_STATUS_HH
