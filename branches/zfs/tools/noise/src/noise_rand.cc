//
// noise_rand.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:59:44 2006 texane
// Last update Thu Feb 23 17:15:58 2006 texane
//


// implement the noiser interface


#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <noise.hh>
#include <buffer.hh>


using namespace std;


rnd::~rnd()
{
}


bool rnd::fuzz(buffer& dst, const buffer& src)
{
  unsigned int nr_rnd;
  unsigned int nr_elem;

  srand((unsigned int)time(0));
  dst = src;
  for (nr_elem = 0; nr_elem < dst.size(); ++nr_elem)
    {
      nr_rnd = rand();
      if ((nr_rnd % 10) == 0)
	dst[nr_elem] = nr_rnd;
    }

  // cout << dst.tostring() << endl;

  return true;
}
