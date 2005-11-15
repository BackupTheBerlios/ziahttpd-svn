//
// cstring_helper.cc<2> for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:11:31 2005 
// Last update Sun Nov 13 15:12:26 2005 
//


#include <dataman/cstring_helper.hh>


char*	dataman::cstring_helper::strcpy(char *dst, const char *src)
{
  while (*src)
    *dst++ = *src++;
  *dst = 0;
  return (dst);
}


char*	dataman::cstring_helper::strcat(char *dst, const char *src)
{
  while (*dst++);
  while (*src)
    *dst++ = *src++;
  *dst = 0;
  return (dst);
}
