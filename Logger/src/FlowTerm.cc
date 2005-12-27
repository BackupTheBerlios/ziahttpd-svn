//
// FlowTerm.cc for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Tue Dec 27 16:14:09 2005 Bigand Xavier
// Last update Tue Dec 27 16:21:46 2005 Bigand Xavier
//

#include "FlowTerm.hh"

FlowTerm::FlowTerm()
{
}

FlowTerm::~FlowTerm()
{
}

bool	FlowTerm::Write(std::string sStr)
{
  std::cout << sStr;
  return true;
}
