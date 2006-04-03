//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 13:31:04 2006 texane
// Last update Mon Apr 03 20:05:27 2006 texane
//


#include "include/mod_encoding.hh"


using namespace std;


IModule* GetNewInstance()
{
  return new mod_encoding;
}
