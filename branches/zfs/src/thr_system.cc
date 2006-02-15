//
// thr_system.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:26 2006 texane
// Last update Wed Feb 15 22:55:00 2006 
//


// This thread is the system one, that's
// it have to perform the pool maintenance.
// This includes reaping blocked threads,
// managing the load...
// This is implemented by polling the pool
// at a regular interval.
// Too, this is the one deallocating slots
// when needed, so that no conccurency problems
// arise (deallocating a thread should only be done
// when there are load problems) -> it can safely
// manipulate allocated slots, since he's the only
// allowed to deallocated them.


#include <ziafs.hh>
#include <cstdio>


void* thr::pool::system_entry(thr::pool::slot_t* thr_slot)
{
  pool::slot_t* slots;
  unsigned int n;

  while (1)
    {
      // Iterate over the slots
      Sleep(100000);
      slots = thr_slot->pool->thr_slots;
      for (n = 0; n < thr_slot->pool->nr_slots; ++n)
	{
	  if (slots[n].allocated == true && slots[n].curr_io.in_progress == true)
	    {
	      printf("[%u] <io_in_progress since %lu> \n", n, thr_slot->pool->nr_ticks - slots[n].curr_io.tm_start);
	      fflush(stdout);
	    }
	}

      // Increment the tick number
      ++thr_slot->pool->nr_ticks;
    }

  return 0;
}
