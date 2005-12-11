//
// CmdLParser.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Sun Dec 11 14:12:33 2005 Bigand Xavier
// Last update Sun Dec 11 15:24:03 2005 Bigand Xavier
//

#ifndef __CmdLParser_H__
#define __CmdLParser_H__

#include <map>
#include <string>
#include <stdio.h>

class CmdLParser
{
private:
  std::map<std::string, std::string>	_mtParam;
  bool					_bOp;

public:
  CmdLParser();
  CmdLParser(int ac, std::string *av);
  ~CmdLParser();

  void	Analyse(int ac, std::string *av);
};

#endif // __CmdLParser_H__
