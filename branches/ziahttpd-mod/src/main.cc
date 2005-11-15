//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 05 11:32:11 2005 texane
// Last update Sun Nov 13 16:47:21 2005 
//


#include <iostream>
#include <zia.hh>


using std::cout;
using std::endl;


int main(int ac, char** av)
{
  server::core core(ac, av);

  cout << "[?] Running server" << endl;
  core.run();
  cout << "[?] Shutting down server" << endl;
}
