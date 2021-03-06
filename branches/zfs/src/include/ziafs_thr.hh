//
// ziafs_thr.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:30:22 2006 texane
// Last update Thu Feb 23 20:35:58 2006 texane
//


#ifndef ZIAFS_THR_HH
# define ZIAFS_THR_HH


// This file covers threads

#include <ziafs_http.hh>
#include <sys/sysapi.hh>

#ifdef _WIN32
# include <time.h>
# include <sys/pthread.h>
#else
# include <pthread.h>
#endif //_WIN32


// Forward declarations
namespace net { class server; }
namespace resource { class handle; };

namespace thr
{
  // Pool is the room where
  // thread are waiting for to
  // be called.
  // At the pool construction, no
  // thread is created.
  // When one's want to assign_task,
  // the pool will walk the thread info
  // set and see wether there are free
  // slot or not, creating.

  // if a thread is not used within a certain
  // amount of time, let it expire (??)
  // don't forget to unlock the slot somewhere

  // implement the thr_printf macro
  // debug

  // Concerning reaper:
  // All possible blocking operations
  // are wrapped into a thr_alterable_read(slot, buffer);
  // this wrapper stores the begin of the operation
  // on return, the function behaves like the normal one,
  // but the following error code is added:
  // e_timeouted
  // the system pool thread will make the other timeout
  // by polling the pool slots.
  // A state should be defined in order to keep track of the
  // operation
  // If an operation timeouted, the corresponding last_error is set
  // in the thread slot, the reaper has to close the file descriptor
  // associated with the operation to make the thread return

  // correct the pthread_detach bug
  // if pthread_detach is called, then pthread_join will
  // fail

  // only the system thread is allowed to remove a thread,
  // so deallocating a thread slot, to tell a thread giving
  // up a resource, a client... by closing descriptors...
  // in order to do so, the system thread can signal the target
  // one (blocked in pthread_cond_wait)

  // The e_timeouted error doesn't indicate the thread has to
  // terminate, just it should give up the current operation.
  // What should be done after that is left to implemententor

  // Information about the inprogress io
  typedef enum
    {
      IO_NONE = 0,
      IO_ACCEPT,
      IO_SEND,
      IO_SENDFILE,
      IO_RECV,
    } io_id_t;
  typedef struct
  {
    unsigned int sz;
    unsigned long tm_start;
    bool in_progress;
    bool timeouted;
    io_id_t id;
    union
    {
      sysapi::insock::handle_t hsock;
    } desc;
  } io_info_t;

  // Thread pool
  class pool
  {
  public:
    pool(unsigned int);
    ~pool();

    unsigned long tm_now() { return nr_ticks; }

    typedef struct slot
    {
      // thread
      volatile bool thr_done;
      volatile bool thr_ready;
      pthread_t thr_id;
      pthread_cond_t cond_start;
      pthread_mutex_t mtx_start;
      void* (*entry_fn)(struct slot*);
      void* uparam;
      thr::pool* pool;

      // current io information
      io_info_t curr_io;

      // blocking operations
      time_t tm_started;
      bool thr_blocked;

      // last error
      typedef enum
	{
	  e_success = 0,
	  e_timeouted,
	  e_connclosed,
	  e_unknown
	} error_t;
      error_t thr_lasterr;

      // thread state
      bool allocated;
      int locked;
    } slot_t;

    // assign task to cached thread
    bool assign_task(void* (*)(slot_t*), void*);

    // Thread entry points
    static void* server_entry(slot_t*);
    static void* system_entry(slot_t*);

    // session management
    typedef struct
    {
      net::server* srv;
      slot_t* thr_slot;
      sysapi::insock::handle_t cli_sock;
      struct sockaddr_in cli_addr;
      bool done;
      bool ret_in_cache;
      net::http proto;
      net::http::chunk_pos_t chunk_pos;
      resource::handle* target;
    } session_t;
    static void sess_reset_request(session_t&);
    static void sess_release_request(session_t&);
    static void sess_reset(session_t&);
    static void sess_release(session_t&);
    static bool sess_bind_server(session_t&);
    static bool sess_accept_connection(session_t&);
    static bool sess_read_metadata(session_t&);
    static bool sess_handle_predata(session_t&);
    static bool sess_handle_request(session_t&);

  private:
    // Thread slots
    slot_t* thr_slots;
    unsigned int nr_slots;

    // Number of ticks since system
    unsigned long nr_ticks;

    // Internals
    void reset_slot(slot_t&);
    bool allocate_slot(slot_t&);
    bool release_slot(slot_t&);
    bool execute_task(slot_t&);
  };


  // io related operations
  void io_info_reset(io_info_t&);
  // for network io
  sysapi::error::handle_t send(pool::slot_t&, sysapi::insock::handle_t&, unsigned char*, unsigned int, unsigned int&);
  sysapi::error::handle_t recv(pool::slot_t&, sysapi::insock::handle_t&, unsigned char*, unsigned int, unsigned int&);
}


#endif // ! ZIAFS_THR_HH
