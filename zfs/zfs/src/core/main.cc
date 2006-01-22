//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 00:16:46 2006 texane
// Last update Sun Jan 22 13:53:15 2006 texane
//


#include <iostream>
#include <core/ziafs.hh>

using namespace status;
using namespace io;

int main(int ac, char** av)
{
  manager man;
  resource* res;
  buffer buf;

  man.create(res, ST_FEEDING, "toto.out");
  while (res->fetch(buf) == SUCCESS)
    {
      std::cout << buf.tostring() << std::endl;
    }

  man.destruct(res);
}
