//
// Logger.cpp for  in
//
// Made by Bigand Xavier
// Login   <@epita.fr>
//
// Started on  Mon Dec 26 12:43:59 2005 Bigand Xavier
// Last update Mon Dec 26 17:39:56 2005 Bigand Xavier
//

#include "Logger.hh"

Logger::Logger()
{
  tStrSize = STR_SIZE;
  bError = false;
}

Logger::~Logger()
{
  int	i;

  for (i = 0; i < NB_TYPE; i++)
    delete Flow[i];
}
