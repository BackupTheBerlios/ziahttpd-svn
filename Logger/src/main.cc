//
// main.cc for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 12:43:42 2005 Bigand Xavier
// Last update Tue Dec 27 17:21:39 2005 Bigand Xavier
//

#include "Logger.hh"
#include "FlowTerm.hh"
#include "FlowFile.hh"
#include <iostream>

#define MAX	1000

int	main(int ac, char **av)
{
  unsigned long	i;

  Logger	test;
  FlowTerm	Flow1;
  FlowFile	Flow2("./tmp.txt");

  test.SetFlow(INFO, Flow1);
  for (i = 0; i < MAX; i++)
    {
      if (i == 10)
	test.SetFlow(INFO, Flow2);
      test.Log(INFO, "prout %d\n", 10);
    }
  system("PAUSE");
  return 0;
}
