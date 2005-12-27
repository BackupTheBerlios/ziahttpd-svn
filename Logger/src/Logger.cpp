//
// Logger.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 12:43:59 2005 Bigand Xavier
// Last update Tue Dec 27 16:27:30 2005 Bigand Xavier
//

#include "Logger.hh"

Logger::Logger()
{
  int	i;

  _tStrSize = STR_SIZE;
  _bError = false;
  for (i = 0; i < NB_TYPE; i++)
    {
      _bFlow[i] = false;
      _Flow[i] = NULL;
    }
}
