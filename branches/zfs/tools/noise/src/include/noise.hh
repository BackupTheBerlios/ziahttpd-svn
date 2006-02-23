//
// noise.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:55:11 2006 texane
// Last update Thu Feb 23 10:31:32 2006 texane
//


#ifndef NOISE_HH
# define NOISE_HH


#include <proxy.hh>
#include <buffer.hh>
#include <sched.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sysapi.hh>


// noise interface
class noise
{
public:
  // destruction
  virtual ~noise() {}

  // logging
  void log(buffer&, buffer&);

  // interface
  virtual bool fuzz() = 0;
};


#endif // ! NOISE_HH
