//
// thr_pool.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 02:00:09 2006 texane
// Last update Tue Feb 14 02:51:50 2006 texane
//


#include <pthread.h>
#include <ziafs_thr.hh>


// Internal

void thr::pool::reset_slot(thr::pool::slot_t& slot)
{
  slot.locked = 0;
  slot.allocated = false;
//   slot.thr_id = ;
}

void thr::pool::enter()
{}

void thr::pool::leave()
{}

static inline bool lock_the_slot(int* lock)
{
  if (*lock == 0)
    {
      *lock = 1;
      return true;
    }
  return false;
}

static inline bool unlock_the_slot(int* lock)
{
  if (*lock == 1)
    {
      *lock = 0;
      return true;
    }
  return false;
}

static inline void allocate_the_slot(thr::pool::slot_t& slot)
{
  slot.locked = 1;
  slot.allocated = true;
}

static inline void release_the_slot(thr::pool::slot_t& slot)
{
  slot.allocated = false;
  slot.locked = 0;
}


// Exported

thr::pool::pool(unsigned int nslots = 10)
{
  nr_slots = nslots;
  thr_slots = new slot_t[nr_slots];
}

thr::pool::~pool()
{
  delete[] thr_slots;
}

bool thr::pool::assign_task(void* (*task_fn)(struct param&), struct param& p)
{
  unsigned int nslot;
  slot_t* p_slot;
  bool done;

  // Find a slot
  p_slot = 0;
  done = false;
  for (nslot = 0; done == false && nslot < nr_slots; ++nslot)
    {
      if (thr_slots[nslot].allocated == true &&
	  lock_the_slot(&thr_slots[nslot].locked))
	{
	  // a slot has been previously locked
	  if (p_slot)
	    unlock_the_slot(&thr_slots[nslot].locked);
	  p_slot = &thr_slots[nslot];
	  done = true;
	}
      else if (p_slot == 0 && thr_slots[nslot].allocated == false)
	{
	  if (lock_the_slot(&thr_slots[nslot].locked))
	    p_slot = &thr_slots[nslot];
	}
    }

  // No slot has been found, allocate one
  if (done == false && p_slot)
    {
      allocate_the_slot(*p_slot);
    }
  // No slot has been found, no more available
  else
    {
      return false;
    }

  return true;
}
