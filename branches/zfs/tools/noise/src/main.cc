//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:54:43 2006 texane
// Last update Thu Feb 23 16:28:34 2006 texane
//


#include <noise.hh>


int main(int ac, char** av)
{
  proxy p(ac, av);
  p.forward();
}
