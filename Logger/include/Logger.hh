//
// Logger.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 12:44:16 2005 Bigand Xavier
// Last update Mon Dec 26 14:51:28 2005 Bigand Xavier
//

#ifndef __Logger_H__
#define __Logger_H__


#include <iostream>
#include <string>

#define L_ALL		1
#define L_ERROR_WARNING	2
#define L_ERROR		3
#define L_NONE		4

#define	LOGGER_STATE	L_ERROR

#define INFO		1
#define WARNING		2
#define ERROR		3

class	Logger
{
private:

public:
  Logger() {};
  ~Logger() {};

  bool	Log(int iType, std::string sStr)
  {
    if (iType >= LOGGER_STATE)
      {
	std::cout << sStr;
      }
    return true;
  };
};

#endif // __Logger_H__
