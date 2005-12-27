//
// ManageFlow.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 17:16:04 2005 Bigand Xavier
// Last update Tue Dec 27 16:13:39 2005 Bigand Xavier
//

#ifndef __ManageFlow_H__
#define __ManageFlow_H__

#include <iostream>
#include <string>

class	ManageFlow
{
public:
  virtual bool	Write(std::string sStr) = 0;
};

#endif // __ManageFlow_H__
