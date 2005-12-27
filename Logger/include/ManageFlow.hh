//
// ManageFlow.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 17:16:04 2005 Bigand Xavier
// Last update Tue Dec 27 15:28:34 2005 Bigand Xavier
//

#ifndef __ManageFlow_H__
#define __ManageFlow_H__

#include <iostream>
#include <string>

#define	MFLOW_UNKOWN	-1
#define MFLOW_NORMAL	0
#define MFLOW_OPEN	1
#define MFLOW_CLOSE	2
#define MFLOW_ERROR	3

class	ManageFlow
{
protected:
  int	_iState;

public :
  virtual bool	Open() = 0;
  virtual bool	Write(std::string sStr) = 0;
  virtual bool	Close() = 0;
  virtual int	GetState() {return _iState;};
};

#endif // __ManageFlow_H__
