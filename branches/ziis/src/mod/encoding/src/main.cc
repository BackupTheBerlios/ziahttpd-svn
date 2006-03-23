//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 13:31:04 2006 texane
// Last update Wed Mar 23 17:23:33 2005 texane
//


#include "include/mod_encoding.hh"


using namespace std;


IModule* GetNewInstance()
{
  cout << "[mod_encoding] GetNewInstance()" << endl;
  return new mod_encoding;
}
