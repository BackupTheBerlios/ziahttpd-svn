//
// FlowFile.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Tue Dec 27 17:10:11 2005 Bigand Xavier
// Last update Tue Dec 27 17:25:37 2005 Bigand Xavier
//

#ifndef __FlowFile_H__
#define __FlowFile_H__


#include <iostream>
#include <string>
#include <fstream>

#include "ManageFlow.hh"

class	FlowFile : public ManageFlow
{
private:
  std::ofstream      Output;

public:
  FlowFile(std::string sFile)
  {
    Output.open(sFile.c_str());
  };

  ~FlowFile() {};

  bool	Write(std::string sStr)
  {
    Output << sStr;
    return true;
  };
};

#endif // __FlowFile_H__
