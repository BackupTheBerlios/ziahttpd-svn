//
// FlowFile.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Tue Dec 27 17:10:11 2005 Bigand Xavier
// Last update Tue Dec 27 17:13:58 2005 Bigand Xavier
//

#ifndef __FlowFile_H__
#define __FlowFile_H__


#include <iostream>
#include <string>

#include "ManageFlow.hh"

class	FlowFile : public ManageFlow
{
public:
  FlowFile() {};
  ~FlowFile() {};

  bool	Write(std::string sStr)
  {

    return true;
  };
};

#endif // __FlowFile_H__
