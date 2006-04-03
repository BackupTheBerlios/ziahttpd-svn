//
// str_utils.cc for  in /home/texane/texane/projects/ziahttpd/branches/ziis/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Apr  3 15:49:25 2006 
// Last update Mon Apr  3 15:49:35 2006 
//


#ifndef _WIN32
static int cicmp(char a, char b)
{
  if (a >= 'A' && a <= 'Z')
    a += 'a' - 'A';
  if (b >= 'A' && b <= 'Z')
    b += 'a' - 'A';
  return b - a;
}

int stricmp(const char* s1, const char* s2)
{
  while (cicmp(*s1, *s2) == 0)
    {
      if (*s1 == 0)
	return 0;
      ++s1;
      ++s2;
    }
  return *s2 - *s1;
}
#endif // !_WIN32
