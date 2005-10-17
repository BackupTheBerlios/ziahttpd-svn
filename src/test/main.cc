//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 05 11:32:11 2005 texane
// Last update Tue Oct 11 11:27:39 2005 texane
//


#include <iostream>
#include <sysapi.hh>

extern void test_file(const char*, const unsigned char*);
extern void test_process();
extern void test_thread();

int main(int ac, char** av)
{
  // test_file("./tmp/foo", reinterpret_cast<const unsigned char*>("This is a simple string..."));
  // test_process();
  test_thread();
}
