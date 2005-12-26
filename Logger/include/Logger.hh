//
// Logger.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 12:44:16 2005 Bigand Xavier
// Last update Mon Dec 26 16:25:09 2005 Bigand Xavier
//

#ifndef __Logger_H__
#define __Logger_H__


#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdio.h>

#ifdef WIN32
#define	vsnprintf	_vsnprintf
#endif

#define L_ALL		1
#define L_ERROR_WARNING	2
#define L_ERROR		3
#define L_NONE		4

#define	LOGGER_STATE	L_ALL

#define INFO		1
#define WARNING		2
#define ERROR		3

#define STR_SIZE	512

class	Logger
{
private:
  size_t	tStrSize;
public:
  Logger();
  ~Logger();

  bool	Log(int iType, std::string sStr, ...)
  {
    if (iType >= LOGGER_STATE)
      {
	va_list	tParam;
	char	*pRes = new char[tStrSize];

	memset(pRes, 0, tStrSize);
	va_start(tParam, sStr);
	vsnprintf(pRes, tStrSize, sStr.c_str(), tParam);
	std::cout << pRes;
	va_end(tParam);
	delete[] pRes;
      }
    return true;
  };
};

#endif // __Logger_H__
