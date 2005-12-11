//
// CmdLParser.cc for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Sun Dec 11 14:11:57 2005 Bigand Xavier
// Last update Sun Dec 11 15:47:39 2005 Bigand Xavier
//

#include "CmdLParser.hh"

CmdLParser::CmdLParser()
{
  _bOp = false;
}

CmdLParser::CmdLParser(int ac, std::string *av)
{
  _bOp = false;
  Analyse(ac, av);
}

CmdLParser::~CmdLParser()
{
}

void	CmdLParser::Analyse(int ac, std::string *av)
{
  int		i;
  char		Key[9];

  for (i = 1; i < ac; i++)
    {
      if (_bOp == false && av[i][0] != '-')
	{
	  sprintf(Key, "%i", i - 1);
	  _mtParam[Key] = av[i];
	}
      else
	{
	  _bOp = true;
	  if (i + 1 < ac && av[i + 1][0] != '-')
	    _mtParam[&av[i][1]] = &av[++i][0];
	  else
	    _mtParam[&av[i][1]] = "1";
	}
    }
}
