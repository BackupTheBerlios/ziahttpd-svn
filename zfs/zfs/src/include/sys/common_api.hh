//
// common_api.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 14:07:22 2006 texane
// Last update Sat Feb 11 18:57:34 2006 
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

	  OPEN_FAILED,
	  CLOSE_FAILED,
	  READ_FAILED,
	  WRITE_FAILED,
	  RESOLV_FAILED,
	  INVALID_NAME,
	  INVALID_DESCRIPTOR,

	  CONNECTION_ABORTED,
	  CONNECTION_CLOSED,

	  // unknown error
	  UNKNOWN
	} errval_t, handle_t;

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
      error::handle_t read(handle_t&, unsigned char*, unsigned int, unsigned int&);
      error::handle_t write(handle_t&, unsigned char*, unsigned int, unsigned int&);
      error::handle_t size(handle_t&, unsigned long long&);
    }


    // internet socket related
    namespace insock
    {
      error::handle_t init_subsystem();
      error::handle_t release_subsystem();
      error::handle_t p_to_inaddr(struct sockaddr_in&, const std::string&, unsigned short);
      error::handle_t n_to_inaddr(struct sockaddr_in&, unsigned long, unsigned short);
      error::handle_t create_listening(handle_t&, struct sockaddr_in&, unsigned int nbklog);
      error::handle_t accept(handle_t&, struct sockaddr_in&, handle_t&);
      error::handle_t recv(handle_t&, unsigned char*, unsigned int, unsigned int&);
      error::handle_t send(handle_t&, unsigned char*, unsigned int, unsigned int&);
      error::handle_t close(handle_t&);
    }


    // modules related
    namespace module
    {
      error::handle_t load(handle_t&, const std::string&);
      error::handle_t unload(handle_t&);
      error::handle_t resolve(void*&, handle_t&, const std::string&);
    }


    // time related
    namespace time
    {
      error::handle_t current(handle_t&);
    }
  }

}


#endif // ! SYS_COMMON_API_HH
