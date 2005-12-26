//
// main.cc for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 12:43:42 2005 Bigand Xavier
// Last update Mon Dec 26 16:51:11 2005 Bigand Xavier
//

#include "Logger.hh"

#include <iostream>

#define MAX	1000

int	main(int ac, char **av)
{
  unsigned long	i;

  Logger	test;

  for (i = 0; i < MAX; i++)
    test.Log(INFO, "prout %d\n", 10);
  system("PAUSE");
  return 0;
}
