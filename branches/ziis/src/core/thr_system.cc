//
// thr_system.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:26 2006 texane
// Last update Wed Mar 22 21:39:44 2006 texane
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


using namespace sysapi;


void* thr::pool::system_entry(thr::pool::slot_t* thr_slot)
{
  core_t* core;
  pool::slot_t* slots;
  unsigned int n;
  unsigned long poll_delay;
  unsigned long expir_delay;

  core = (core_t*)thr_slot->uparam;
  expir_delay = core->config->get_system()->expiration_delay;
  poll_delay = core->config->get_system()->poll_delay;

  while (1)
    {
      // Iterate over the slots
      Sleep(poll_delay);
      slots = thr_slot->pool->thr_slots;
      for (n = 0; n < thr_slot->pool->nr_slots; ++n)
	{
	}

      // Increment the tick number
      ++thr_slot->pool->nr_ticks;
    }

  return 0;
}
