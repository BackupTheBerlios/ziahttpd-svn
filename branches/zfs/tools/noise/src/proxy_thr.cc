//
// proxy_thr.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 10:58:42 2006 texane
// Last update Thu Feb 23 11:17:23 2006 texane
//


#include <iostream>
#include <noise.hh>


using namespace std;
using namespace sysapi;


void* proxy::thr_entry(request_t* req)
{
  cout << "[ ] new thread" << endl;
  cout << "[x] new thread" << endl;
  return 0;
}
