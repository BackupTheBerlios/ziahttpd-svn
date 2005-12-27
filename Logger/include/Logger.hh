//
// Logger.hh for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 12:44:16 2005 Bigand Xavier
// Last update Tue Dec 27 15:33:27 2005 Bigand Xavier
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

#define UNKNOWN		-1
#define WRONG_TYPE	0
#define	WRITE_ERROR	1
#define OPEN_ERROR	2

class	Logger
{
private:
  size_t	_tStrSize;
  bool		_bError;
  int		_iError;
  ManageFlow	*_Flow[NB_TYPE];

public:
  Logger();
  ~Logger();

  void		Log(int iType, std::string sStr, ...)
  {
    if (iType >= LOGGER_STATE)
      {
	if (iType >= NB_TYPE || iType < 0)
	  {
	    _bError = true;
	    _iError = WRONG_TYPE;
	    return ;
	  }

	va_list	tParam;
	char	*pRes = new char[_tStrSize];

	memset(pRes, 0, _tStrSize);
	va_start(tParam, sStr);
	vsnprintf(pRes, _tStrSize, sStr.c_str(), tParam);
	_bError = _Flow[iType]->Write(pRes);
	if (_bError)
	  _iError = WRITE_ERROR;
	va_end(tParam);
	delete[] pRes;
      }
  };

  void		SetFlow(int iType, ManageFlow &Flow)
  {
    if (iType >= LOGGER_STATE)
      {
	if (iType >= NB_TYPE || iType < 0)
	  {
	    _bError = true;
	    _iError = WRONG_TYPE;
	    return;
	  }
	delete _Flow[iType];
	_Flow[iType] = &Flow;
	if (!_Flow[iType]->Open())
	  {
	    _bError = true;
	    _iError = OPEN_ERROR;
	  }
      }
  }

  bool		Error()
  {
    if (!_bError)
      return false;
    _bError = false;
    return true;
  };


  bool		SetMaxLen(size_t tLen)
  {
    if (tLen <= 0)
      return false;
    _tStrSize = tLen;
    return true;
  };

  int		GetError() {return _iError;};
  size_t	GetMaxLen() {return _tStrSize;};
};

#endif // __Logger_H__
