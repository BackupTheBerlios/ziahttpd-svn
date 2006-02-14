//
// thr_pool.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 02:00:09 2006 texane
// Last update Tue Feb 14 23:29:59 2006 texane
//


#include <ziafs.hh>


// thread cache function

void* pool_cache_entry(void* param)
{
  thr::pool::slot_t* p_slot;
  int err;

  p_slot = (thr::pool::slot_t*)param;

  // Lock the mutex
  err = 1;
  while (err)
    err = pthread_mutex_lock(&p_slot->mtx_start);

  // Execute the task
  while (p_slot->thr_done == false)
    {
      p_slot->thr_ready = true;
      err = pthread_cond_wait(&p_slot->cond_start, &p_slot->mtx_start);
      p_slot->thr_ready = false;
      if (err)
	{
	  p_slot->thr_ready = true;
	}
      else if (p_slot->entry_fn)
	{
	  p_slot->entry_fn(p_slot);
	}
      p_slot->locked = 0;
    }
  pthread_mutex_unlock(&p_slot->mtx_start);
  return 0;
}


// Internal pool slot management

void thr::pool::reset_slot(thr::pool::slot_t& slot)
{
  slot.locked = 0;
  slot.allocated = false;
  slot.thr_done = false;
  slot.thr_ready = false;
  slot.thr_blocked = false;
  slot.thr_lasterr = slot_t::e_success;
  slot.pool = this;
  io_info_reset(slot.curr_io);
//   slot.cond_start = 0;
//   slot.mtx_start = 0;
  slot.entry_fn = 0;
  slot.uparam = 0;
}

bool thr::pool::allocate_slot(thr::pool::slot_t& slot)
{
  // Create a thread that's
  // blocked on a condition
  // variable.

  bool ret;
  int err;

  ret = true;

//   slot.cond_start = 0;
//   slot.mtx_start = 0;

  err = pthread_mutex_init(&slot.mtx_start, 0);
  if (err)
    {
      ret = false;
      goto end_of_allocate;
    }

  err = pthread_cond_init(&slot.cond_start, 0);
  if (err)
    {
      ret = false;
      goto end_of_allocate;
    }
  slot.allocated = true;

  err = pthread_create(&slot.thr_id, 0, pool_cache_entry, (void*)&slot);
  if (err)
    {
      ret = false;
      goto end_of_allocate;
    }

 end_of_allocate:
  if (ret == false)
    {
      release_slot(slot);
      reset_slot(slot);
    }

  return ret;
}

bool thr::pool::release_slot(thr::pool::slot_t& slot)
{
  int ret;
  int* p_ret = &ret;

  if (slot.allocated == false)
    return false;

//   if (slot.mtx_start)
    pthread_mutex_destroy(&slot.mtx_start);

//   if (slot.cond_start)
    pthread_cond_destroy(&slot.cond_start);

  // ??
  // pthread_detach(slot.thr_id);
  pthread_join(slot.thr_id, (void**)&p_ret);
  return true;
}

bool thr::pool::execute_task(thr::pool::slot_t& slot)
{
  unsigned int ntry;
  int err;

  // Wait for the thread
  ntry = 0;
  while (slot.thr_ready == false)
    ++ntry;
  Sleep(0);
  err = pthread_cond_signal(&slot.cond_start);
  if (err)
    return false;
  return true;
}


#ifdef _WIN32
static inline bool lock_the_slot(int* the_lock)
{
  __asm
    {
      mov edx, dword ptr[the_lock]
      xor eax, eax
      xor ecx, ecx
      inc ecx
      lock cmpxchg dword ptr[edx], ecx
      jnz already_locked
    }
  return true;
 already_locked:
  return false;
}
#else
static inline bool lock_the_slot(int* the_lock)
{
  unsigned char owner;

  owner = 0;
  __asm__ __volatile__ ("xorl %%eax, %%eax\n\t"
			"xorl %%ecx, %%ecx\n\t"
			"incl %%ecx\n\t"
 			"lock cmpxchg %%ecx, %0\n\t"
			"jnz already_locked\n\t"
			"incl %1\n\t"
			"already_locked:\n\t"
			: "=m"(*the_lock), "=m"(owner)
			:
			: "eax", "ecx");
  if (owner == 1)
    return true;
  return false;
}
#endif // _WIN32

static inline bool unlock_the_slot(int* lock)
{
  *lock = 0;
  return true;
}



// Exported

thr::pool::pool(unsigned int nslots = 10)
{
  unsigned int n;

  nr_ticks = 0;
  nr_slots = nslots;
  thr_slots = new slot_t[nr_slots];
  for (n = 0; n < nslots; ++n)
    reset_slot(thr_slots[n]);
}

thr::pool::~pool()
{
  delete[] thr_slots;
}

bool thr::pool::assign_task(void* (*entry_fn)(slot_t*), void* uparam)
{
  unsigned int nslot;
  slot_t* p_slot;
  bool done;

  // Find a slot
  p_slot = 0;
  done = false;
  for (nslot = 0; done == false && nslot < nr_slots; ++nslot)
    {
      if (lock_the_slot(&thr_slots[nslot].locked) == true)
	{
	  if (thr_slots[nslot].allocated == true)
	    {
	      if (p_slot)
		unlock_the_slot(&p_slot->locked);
	      p_slot = &thr_slots[nslot];
	      done = true;
	    }
	  else if (p_slot == 0)
	    {
	      p_slot = &thr_slots[nslot];
	    }
	  else
	    {
	      unlock_the_slot(&thr_slots[nslot].locked);
	    }
	}
    }

  // no more slot available
  if (done == false && p_slot == 0)
    return false;

  // No slot has been found, allocate one
  if (done == false)
    allocate_slot(*p_slot);

  // Fill the slot
  p_slot->entry_fn = entry_fn;
  p_slot->uparam = uparam;

  // Start the thread
  execute_task(*p_slot);

  return true;
}
