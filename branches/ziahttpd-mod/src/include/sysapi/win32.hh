//
// win32.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 09 13:56:50 2005 texane
// Last update Wed Nov 23 22:07:28 2005 texane
//

#ifndef SYSAPI_WIN32_HH
# define SYSAPI_WIN32_HH

// @see sysapi.hh
// This file declares the namespace
// used for the win32 system api.

#include <string>
//#include <winsock2.h>
#include <windows.h>
#include <process.h>


namespace win32
{
  // @see sysapi/win32/file.cc
  namespace file
  {
    // File path component delimitor
    static const char delim = '\\';

    // Openning modes
    typedef enum
      {
	RDONLY = GENERIC_READ,
	WRONLY = GENERIC_WRITE,
	RDWR   = GENERIC_READ | GENERIC_WRITE
      } mode_t;

    // Seek position
    typedef enum
    {
      BEGIN   = FILE_BEGIN,
      CURRENT = FILE_CURRENT,
      END     = FILE_END
    } pos_t;

    typedef HANDLE handle_t;
    typedef DWORD offset_t;
    typedef DWORD size_t;

    // Function prototypes
    bool	open(handle_t*, const char*, mode_t, int* = static_cast<int*>(0));
    bool	create(const char*, mode_t = RDWR, handle_t* = static_cast<handle_t*>(0), int* = static_cast<int*>(0));
    bool	close(handle_t, int* = static_cast<int*>(0));
    bool	close_rd(handle_t, int* = static_cast<int*>(0));
    bool	close_wr(handle_t, int* = static_cast<int*>(0));
    bool	read(handle_t, unsigned char*, size_t, size_t* = static_cast<size_t*>(0), int* = static_cast<int*>(0));
    bool	write(handle_t, const unsigned char*, size_t, size_t* = static_cast<size_t*>(0), int* = static_cast<int*>(0));
    bool	seek(handle_t, offset_t, pos_t = CURRENT, int* = static_cast<int*>(0));
    
    // Access rights functions
    bool	exists(const char*);
    bool	size(const char*, unsigned long*);
    bool	is_directory(const char*);
    bool	is_readable(const char*);
    bool	is_writtable(const char*);
    bool	is_executable(const char*);
  }


  // @see sysapi/win32/process.cc
  namespace process
  {
    // options for the wait function
    typedef enum
      {
	DONTWAIT = 0,	// returns if no pending process
	WAITFOREVER	// wait until process comes
      } waitopt_t;

    // State of the process
    typedef enum
      {
	RUNNING = 0,
	BLOCKED,
	TERMINATED
      } state_t;

    // Signal that can be sent to the process
    typedef enum
      {
	TERMINATE = 0
      } sigid_t;

    // Handle on the process
    typedef HANDLE handle_t;

    // Public interface
    bool	create_and_loadexec(handle_t*, int, const char**, const char** = NULL);
    bool	create_outredir_and_loadexec(handle_t*, win32::file::handle_t*, int, const char**, const char** = NULL);
    bool	create_inoutredir_and_loadexec(handle_t*, win32::file::handle_t*, win32::file::handle_t*, int, const char**, const char** = NULL);
    bool	myhandle(handle_t*);
    bool	signal(handle_t, sigid_t);
    bool	release(handle_t);
    bool	wait_single(handle_t, state_t* = NULL, waitopt_t = WAITFOREVER);
    bool	wait_any(handle_t* = NULL, state_t* = NULL, waitopt_t = WAITFOREVER);
  }


  // @see sysapi/win32/thread.cc
  namespace thread
  {
    // Handle on threads
    typedef HANDLE handle_t;

    // Thread entry point
    typedef void* param_t;
    typedef unsigned retcode_t;
    typedef retcode_t (*entry_t)(param_t);

    // thread states
    typedef enum
      {
	RUNNING = 0,
	SUSPENDED,
	BLOCKED,
	TERMINATED
      } state_t;

    // wait options
    typedef enum
      {
	DONTWAIT = 0,
	WAITFOREVER
      } waitopt_t;

    // Signal id
    typedef enum
      {
	TERMINATE = 0,
	RESUME,
	SUSPEND
      } sigid_t;


    // Public interface
    bool	create_and_exec(handle_t*, entry_t, param_t = NULL);
    bool	attach_to_process(handle_t, win32::process::handle_t);
    bool	detach_from_process(handle_t, win32::process::handle_t);
    bool	wait_single(handle_t, state_t* = NULL, waitopt_t = WAITFOREVER);
    bool	myhandle(handle_t*);
    bool	release(handle_t);
    bool	say(const char* = NULL);
  }


  // @see sysapi/win32/socket.cc
  namespace socket_in
  {
    // System dependant errors
    typedef enum
      {
	// Subsystem related
	SUBSYSTEM_FAILED = 0,
	
	// Socket related
	SOCKET_NOT_CONNECTED,
	SOCKET_INVALID_HANDLE,
	SOCKET_TOO_MANY_HANDLE,
	SOCKET_PERM_DENIED,

	// Internet address related
	INADDR_IS_INVALID,

	// System calls / functions related
	CALL_WOULDBLOCK,
	CALL_INPROGRESS,
	CALL_INTERRUPTED,

	// Connection related
	CONN_ABORTED,  // closed by the host software
	CONN_RESET,    // connection aborted by the foreign soft
	CONN_REFUSED,
	CONN_DISCONNECTED, // gracefully disconnected

	// Unknown error
	ERR_UNKNOWN,

	// Success
	SUCCESS
      } error_t;

    // System dependant type
    typedef SOCKET handle_t;
    typedef int size_t;

    // Interface
    bool init_subsystem(win32::socket_in::error_t* = NULL);
    bool release_subsystem(win32::socket_in::error_t* = NULL);
    bool create_listening(win32::socket_in::handle_t*,
			  unsigned short,
			  unsigned long = INADDR_ANY,
			  int = 0,
			  win32::socket_in::error_t* = NULL);
    bool create_listening(win32::socket_in::handle_t*,
			  unsigned short,
			  const char*,
			  int = 0,
			  win32::socket_in::error_t* = NULL);
    bool accept(win32::socket_in::handle_t*,
		win32::socket_in::handle_t,
		struct sockaddr* = NULL,
		win32::socket_in::error_t* = NULL);
    bool terminate_connection(win32::socket_in::handle_t);
    bool recv(win32::socket_in::handle_t,
	      unsigned char*,
	      win32::socket_in::size_t,
	      win32::socket_in::size_t* = NULL,
	      win32::socket_in::error_t* = NULL);
    bool send(win32::socket_in::handle_t,
	      const unsigned char*,
	      win32::socket_in::size_t,
	      win32::socket_in::size_t* = NULL,
	      win32::socket_in::error_t* = NULL);
  }


  // @see sysapi/win32/mutex.cc
  namespace mutex
  {
    // Handle on the mutex
    typedef HANDLE handle_t;

    // Interface
    bool create(win32::mutex::handle_t*);
    bool destroy(win32::mutex::handle_t);
    bool acquire(win32::mutex::handle_t);
    bool release(win32::mutex::handle_t);
  }


  // @see sysapi/win32/error.cc
  namespace error
  {
    typedef DWORD handle_t;

    handle_t	get();
    void	set(handle_t);
    void	stringify(const char* = NULL);
  }


  // @see sysapi/win32/shared_object.cc
  namespace shared_object
  {
    typedef HMODULE handle_t;

    typedef enum
      {
      } error_t;

    bool	open(handle_t&, const std::string&, error_t&);
    bool	close(handle_t&);
    bool	resolve(void*&, handle_t&, const std::string&, error_t&);
  }

}



// - Doxygen comments start from here

//! \file
//! \brief Implements the win32 part of sysapi
//!
//! Interfaces available for file, process, thread.
//! See the files win32.hh for more informations.


#endif // ! SYSAPI_WIN32_HH
