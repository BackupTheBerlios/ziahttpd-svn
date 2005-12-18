//
// main.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Sat Oct 22 10:46:59 2005 Bigand Xavier
// Last update Sun Dec 18 13:39:19 2005 Bigand Xavier
//

#include <iostream>
#include "ConfManager.hh"
#include "Getopt.hh"

int		main(int ac, char **av)
{
  std::string	file("./conf/zia.conf");
  ConfManager	Conf(ac, av, *file.c_str());
  tStringVector	var;
  int		i;


//  std::cout << "port : " << Conf.GetSimpleString("port") << std::endl;

//   std::cout << "TimeOut : " << Conf.GetSimpleString("TimeOut") << std::endl;
//   Conf.SetSimpleString("TimeOut", "512");
//   std::cout << "modified TimeOut : " << Conf.GetSimpleString("TimeOut") << std::endl;
//   std::cout << "timeout : " << Conf.GetSimpleString("timeout") << std::endl;
//   var = Conf.GetListVector("UnusedPort");

//   for (i = 0; i < var.size(); i++)
//     {
//       std::cout << "UnusedPort : " << var.at(i) << std::endl;
//       var.at(i) = "512";
//     }
//   Conf.SetListVector("UnusedPort", var);
//   var = Conf.GetListVector("UnusedPort");
//   for (i = 0; i < var.size(); i++)
//     {
//       std::cout << "modified UnusedPort : " << var.at(i) << std::endl;
//       var.at(i) = "512";
//     }



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










  int	c;

  while ((c = getopt(ac, av, "aBn:")) != -1)
    {
      switch (c)
	{
	case 'a':
	  std::cout << "option a\n";
	  break;
	case 'B':
	  std::cout << "option B\n";
	  break;
	case 'n':
	  std::cout << "option n: value=" << atoi(optarg) << std::endl;
	  break;
	case '?':
	  std::cout << "ERROR: illegal option " << av[optind-1] << std::endl;
	  break;
	default:
	  std::cout << "WARNING: no handler for option " << c << std::endl;
	  break;
	}
    }










  system("PAUSE");
}
