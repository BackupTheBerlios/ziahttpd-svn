/*
** loadcpu.c for  in 
** 
** Made by texane
** Login   <texane@gmail.com>
** 
** Started on  Sun Jan 29 12:00:22 2006 texane
** Last update Sun Jan 29 12:16:29 2006 texane
*/

#include <windows.h>


int main(int ac, char** av)
{
  int i;
  int n;
  char* toto;

  i = 0;

  while (1)
    {
      toto = malloc(1000);
      free(toto);
      ++i;
    }
  return 0;
}
