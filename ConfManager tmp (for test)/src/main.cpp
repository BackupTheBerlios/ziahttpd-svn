//
// main.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Sat Oct 22 10:46:59 2005 Bigand Xavier
// Last update Tue Nov 08 16:14:59 2005 Bigand Xavier
//

#include <iostream>
#include "ConfManager.hh"

int		main(int ac, char **av)
{
  ConfManager	Conf(av);
  tStringVector	var;
  int		i;


  std::cout << "TimeOut : " << Conf.GetSimpleString("TimeOut") << std::endl;
  Conf.SetSimpleString("TimeOut", "512");
  std::cout << "modified TimeOut : " << Conf.GetSimpleString("TimeOut") << std::endl;
  std::cout << "timeout : " << Conf.GetSimpleString("timeout") << std::endl;
  var = Conf.GetListVector("UnusedPort");

  for (i = 0; i < var.size(); i++)
    {
      std::cout << "UnusedPort : " << var.at(i) << std::endl;
      var.at(i) = "512";
    }
  Conf.SetListVector("UnusedPort", var);
  var = Conf.GetListVector("UnusedPort");
  for (i = 0; i < var.size(); i++)
    {
      std::cout << "modified UnusedPort : " << var.at(i) << std::endl;
      var.at(i) = "512";
    }

//   Conf.Reload();

//   std::cout << std::endl << "after reload" << std::endl;
//   std::cout << "TimeOut : " << Conf.GetSimpleString("TimeOut") << std::endl;
//   std::cout << "timeout : " << Conf.GetSimpleString("timeout") << std::endl;
//   var = Conf.GetListVector("UnusedPort");
//   for (i = 0; i < var.size(); i++)
//     {
//       std::cout << "UnusedPort : " << var.at(i) << std::endl;
//       var.at(i) = "512";
//     }


  system("PAUSE");
}
