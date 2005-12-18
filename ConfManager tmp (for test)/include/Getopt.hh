/*
** Getopt.h for  in
**
** Made by Bigand Xavier
** Login   <@epita.fr>
**
** Started on  Sun Dec 18 12:44:04 2005 Bigand Xavier
** Last update Sun Dec 18 13:24:45 2005 Bigand Xavier
*/

#ifndef __Getopt_H__
#define __Getopt_H__

#include <stdio.h>
#include <string.h>
//#include <tchar.h>

extern int	optind;
extern int	opterr;
extern char	*optarg;

int	getopt(int argc, char *argv[], char *optstring);

#endif // __Getopt_H__
