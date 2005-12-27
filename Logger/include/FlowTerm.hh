//
// FlowTerm.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Tue Dec 27 16:10:42 2005 Bigand Xavier
// Last update Tue Dec 27 17:08:33 2005 Bigand Xavier
//

#ifndef __FlowTerm_H__
#define __FlowTerm_H__

#include <iostream>
#include <string>

#include "ManageFlow.hh"

class	FlowTerm : public ManageFlow
{
public:
  FlowTerm() {};
  ~FlowTerm() {};

  bool	Write(std::string sStr)
  {
    std::cout << sStr;
    return true;
  };
};

#endif // __FlowTerm_H__
