/*
** main.c for  in 
** 
** Made by texane
** Login   <texane@gmail.com>
** 
** Started on  Sat Jan 28 20:52:31 2006 texane
** Last update Sun Jan 29 00:14:46 2006 texane
*/


#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <psmon.h>

int main(int ac, char** av)
{
  unsigned int n;
  unsigned char* ptr;
  unsigned int i;

  n = 0;

  while (n < 10)
    {
      ptr = malloc(20000);
      for (i = 0; i < 400; ++i)
	ptr[i] = 0;
      Sleep(200);
    }
}
