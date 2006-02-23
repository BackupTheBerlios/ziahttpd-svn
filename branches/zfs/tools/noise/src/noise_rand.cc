//
// noise_rand.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:59:44 2006 texane
// Last update Thu Feb 23 11:12:54 2006 texane
//


// implement the noiser interface


#include <noise.hh>
#include <buffer.hh>


rnd::~rnd()
{
}


bool rnd::fuzz(buffer& buf)
{
  return false;
}
