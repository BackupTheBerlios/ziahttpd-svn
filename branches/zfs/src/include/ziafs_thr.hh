//
// ziafs_thr.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:30:22 2006 texane
// Last update Tue Feb 14 02:52:46 2006 texane
//


#ifndef ZIAFS_THR_HH
# define ZIAFS_THR_HH


// This file covers threads


#include <pthread.h>


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

  // derive a thread from the
  // basic one (only the entry 
  // function changes).

  // implement the is_locked
  // function.
  // implement the find_next_assignable

  // delay the thread creation

  // if a thread is not used within a certain
  // amount of time, let it expire (??)

  // implement the lock routine
  // implement the thread creation/deletion

  struct param;
  class pool
  {
  public:
    pool(unsigned int);
    ~pool();
    bool assign_task(void* (*)(struct param&), struct param&);

    typedef struct
    {
      pthread_t thr_id;
      bool allocated;
      int locked;
    } slot_t;

  private:
    slot_t* thr_slots;
    unsigned int nr_slots;

    // Internals
    void reset_slot(slot_t&);
    void enter();
    void leave();
  };


  typedef struct param
  {
    pool* thr_pool;
    pool::slot_t* thr_slot;
  } param_t;
}


#endif // ! ZIAFS_THR_HH
