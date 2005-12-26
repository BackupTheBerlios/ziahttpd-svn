//
// Logger.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 12:44:16 2005 Bigand Xavier
// Last update Mon Dec 26 17:45:26 2005 Bigand Xavier
//

#ifndef __Logger_H__
#define __Logger_H__

#include <iostream>
#include <string>
#include <stdarg.h>
#include <stdio.h>

#include "ManageFlow.hh"

#ifdef WIN32
#define	vsnprintf	_vsnprintf
#endif

#define L_ALL		0
#define L_ERROR_WARNING	1
#define L_ERROR		2
#define L_NONE		3

#define	LOGGER_STATE	L_ERROR

#define INFO		0
#define WARNING		1
#define ERROR		2

#define NB_TYPE		3

#define STR_SIZE	512

class	Logger
{
private:
  size_t	tStrSize;
  bool		bError;
  ManageFlow	*Flow[NB_TYPE];

public:
  Logger();
  ~Logger();

  void	Log(int iType, std::string sStr, ...)
  {
    if (iType >= LOGGER_STATE)
      {
	if (iType >= NB_TYPE || iType < 0)
	  {
	    bError = true;
	    return ;
	  }

	va_list	tParam;
	char	*pRes = new char[tStrSize];

	memset(pRes, 0, tStrSize);
	va_start(tParam, sStr);
	vsnprintf(pRes, tStrSize, sStr.c_str(), tParam);
	bError = Flow[iType]->Write(pRes);
	//std::cout << pRes;
	va_end(tParam);
	delete[] pRes;
      }
  };

  bool	Error()
  {
    if (!bError)
      return false;
    bError = false;
    return true;
  };

  size_t	GetMaxLen() {return tStrSize;};

  bool		SetMaxLen(size_t tLen)
  {
    if (tLen <= 0)
      return false;
    tStrSize = tLen;
    return true;
  };
};

#endif // __Logger_H__
