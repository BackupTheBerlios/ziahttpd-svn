//
// ziafs_thr.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:30:22 2006 texane
// Last update Tue Feb 14 18:13:17 2006 texane
//


#ifndef ZIAFS_THR_HH
# define ZIAFS_THR_HH


// This file covers threads


#ifdef _WIN32
# include <time.h>
# include <sys/pthread.h>
#else
# include <pthread.h>
#endif //_WIN32


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

  // see for the initial attributes
  // (scheduling policy, stack size...)

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

  // Server struct should be disassociated from the thread
  // one: a thread has a server, not is a server

  // Think about caching optimization

  // only the system thread is allowed to remove a thread,
  // so deallocating a thread slot, to tell a thread giving
  // up a resource, a client... by closing descriptors...
  // in order to do so, the system thread can signal the target
  // one (blocked in pthread_cond_wait)

  // The e_timeouted error doesn't indicate the thread has to
  // terminate, just it should give up the current operation.
  // What should be done after that is left to implemententor


  class pool
  {
  public:
    pool(unsigned int);
    ~pool();

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

      // ?? should be moved in net::server
      // connection related
      // struct sockaddr_in local_addr;
      // struct sockaddr_in remote_addr;
      // int accept_fd;
      // int client_fd;
      // conf_t conf;
      // net::server* server;
      // server state
//       typedef enum
// 	{
// 	  st_accepting = 0,
// 	  st_rcv_metadata,
// 	  st_snd_metadata,
// 	  st_rcv_data,
// 	  st_snd_data
// 	} state_t;
//       state_t st_server;

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

  private:
    slot_t* thr_slots;
    unsigned int nr_slots;

    // Internals
    void reset_slot(slot_t&);
    bool allocate_slot(slot_t&);
    bool release_slot(slot_t&);
    bool execute_task(slot_t&);
  };
}


#endif // ! ZIAFS_THR_HH
