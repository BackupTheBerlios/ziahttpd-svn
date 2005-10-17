//
// test_thread.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Oct 11 11:28:01 2005 texane
// Last update Tue Oct 11 14:39:50 2005 texane
//

#include <iostream>
#include <sysapi.hh>


#define NR_THREAD	10
static struct thread_info
{
  int nr_thread_;
  bool done_;
  sysapi::thread::handle_t hdl_;
} th_arr[NR_THREAD];


static sysapi::thread::retcode_t thread_entry_point(sysapi::thread::param_t p)
{
  struct thread_info* thinf = reinterpret_cast<struct thread_info*>(p);
  std::cout << "Hello, I m the thread number " << thinf->nr_thread_ << "\n";
  return 0;
}

static void create_new_thread(int nr_thread)
{
  th_arr[nr_thread].done_ = false;
  th_arr[nr_thread].nr_thread_ = nr_thread;
  if (sysapi::thread::create_and_exec(&th_arr[nr_thread].hdl_,
				      sysapi::thread::SUSPENDED,
				      reinterpret_cast<sysapi::thread::entry_t>(thread_entry_point),
				      reinterpret_cast<sysapi::thread::param_t>(&th_arr[nr_thread])) == false)
    {
      sysapi::error::stringify("Thread creation: ");
    }
  else
    {
      std::cout << "new thread created\n";
    }
}

static void create_thread_spool(int nr_thread)
{
  while (--nr_thread >= 0)
    {
      create_new_thread(nr_thread);
    }
}

static void schedule_thread_spool(int nr_thread)
{
  while (--nr_thread >= 0)
    {
      if ((sysapi::thread::signal(th_arr[nr_thread].hdl_, sysapi::thread::RESUME)) == false)
	{
	  sysapi::error::stringify("Resuming thread:");
	  exit(-1);
	}
      if (sysapi::thread::wait_single(th_arr[nr_thread].hdl_, 0, sysapi::thread::WAITFOREVER))
	sysapi::thread::release(th_arr[nr_thread].hdl_);
      else
	sysapi::error::stringify("Cannot wait_single: ");
    }
}

static void waitfor_thread_spool(int nr_thread)
{
  sysapi::thread::state_t st;
  int nr_thread_done;
  int i;

  // Let all the threads in the spool start
  nr_thread_done = nr_thread;
  while (--nr_thread_done >= 0)
    {
      if ((sysapi::thread::signal(th_arr[nr_thread_done].hdl_, sysapi::thread::RESUME)) == false)
	{
	  sysapi::error::stringify("Resuming thread:");
	  exit(-1);
	}
    }

  std::cout << "All thread have been created\n";

  // Wait for all the thread to terminate
  nr_thread_done = 0;
  while (nr_thread_done < nr_thread)
    {
      for (i = 0; i < nr_thread; ++i)
	if (th_arr[i].done_ == false)
	  if (sysapi::thread::wait_single(th_arr[i].hdl_, &st, sysapi::thread::WAITFOREVER))
	    {
	      sysapi::thread::release(th_arr[i].hdl_);
	      th_arr[i].done_ = true;
	      ++nr_thread_done;
	    }
	  else
	    {
	      sysapi::error::stringify("Cannot wait_single: ");
	    }
    }

  std::cout << "All the thread have been done\n";
}


void	test_thread()
{
  // create 10 threads displaying their ids
  create_thread_spool(NR_THREAD);

  // Non anarchic mode, all thread one after the other write to std::cout
  schedule_thread_spool(NR_THREAD);

  // Anarchic mode, all threads write to std::cout
  // waitfor_thread_spool(NR_THREAD);
}
