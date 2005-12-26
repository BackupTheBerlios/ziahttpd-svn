//
// ManageFlow.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 17:16:04 2005 Bigand Xavier
// Last update Mon Dec 26 17:26:16 2005 Bigand Xavier
//

#ifndef __ManageFlow_H__
#define __ManageFlow_H__

#include <iostream>
#include <string>

class	ManageFlow
{
protected:

public :
  bool	open(std::string sStr) = 0;
  bool	write(std::string sStr) = 0;
  bool	close() = 0;
};

#endif // __ManageFlow_H__
