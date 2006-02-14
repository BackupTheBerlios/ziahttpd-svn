//
// thr_system.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:26 2006 texane
// Last update Tue Feb 14 16:35:28 2006 texane
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


#include <ziafs_debug.hh>
#include <ziafs_thr.hh>
#include <windows.h>
#include <cstdio>


void* thr::pool::system_entry(thr::pool::slot_t* thr_slot)
{
  pool::slot_t* slots;
  unsigned int n;

  while (1)
    {
      // Iterate over the slots
      Sleep(100);
      slots = thr_slot->pool->thr_slots;
      for (n = 0; n < thr_slot->pool->nr_slots; ++n)
	{
	  if (slots[n].allocated == true)
	    {
	      printf("[%d] -> \n", n);
	      fflush(stdout);
	    }
	}
    }

  return 0;
}
