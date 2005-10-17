//
// cstring_helper.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Oct 13 18:43:39 2005 texane
// Last update Thu Oct 13 18:44:28 2005 texane
//


#include <cstring_helper.hh>


char*	cstring_helper::strcpy(char *dst, const char *src)
{
  while (*src)
    *dst++ = *src++;
  *dst = 0;
  return (dst);
}

char*	cstring_helper::strcat(char *dst, const char *src)
{
  while (*dst++);
  while (*src)
    *dst++ = *src++;
  *dst = 0;
  return (dst);
}
