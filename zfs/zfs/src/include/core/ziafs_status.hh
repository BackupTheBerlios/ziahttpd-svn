//
// ziafs_status.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:22:54 2006 texane
// Last update Thu Jan 26 01:12:20 2006 texane
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
      PARTIALIMPL,
      BADMODE,
      CANNOT_OPEN,
      CANNOT_READ,
      CANNOT_WRITE,
      CANNOT_CLOSE,
      NOT_FOUND,
      EXPIRED,
			//http
			STATUSLINE_FAILED,
			STATUSLINE_SUCCESS,
			HEADERLINE_FAILED,
			HEADERLINE_SUCCESS,
      INIT_FAILED,
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


#include <list>
#include <iostream>
#include <core/ziafs_buffer.hh>

extern std::list<std::string> ziafs_internal_dump_list;

// Print a stringified object on cerr
#ifdef ZIAFS_PRINT_OBJECTS
# define ziafs_print_object( o )		\
do						\
{						\
  buffer __buf;					\
  (o).dump(__buf);				\
  std::cerr << __buf.tostring();		\
}						\
while (0)
#else
# define ziafs_print_object( o ) do {} while (0)
#endif // ZIAFS_PRINT_OBJECTS


// Push the object in the dump history
#ifdef ZIAFS_DUMP_OBJECTS
# define ziafs_dump_object( o )				\
do							\
{							\
  buffer __buf;						\
  (o).dump(__buf);					\
  ziafs_internal_list.push_front(__buf.tostring());	\
}							\
while (0)
#else
# define ziafs_dump_object( o ) do { } while (0)
#endif // ZIAFS_DUMP_OBJECTS


#endif // ! ZIAFS_STATUS_HH
