/*
** psmon.h for  in 
** 
** Made by texane
** Login   <texane@gmail.com>
** 
** Started on  Sat Jan 28 23:12:20 2006 texane
** Last update Sun Jan 29 00:45:31 2006 texane
*/


#ifndef PSMON_H
# define PSMON_H


typedef unsigned long ps_id_t;
typedef HANDLE ps_handle_t;
typedef unsigned long ps_status_t;
typedef enum { false = 0, true} bool_t;
typedef unsigned long long ps_timestp_t;

typedef struct proc_info
{
  /* time related */
  ps_timestp_t tmstp;

  /* processor load */
  unsigned long cpu_usage;

  /* memory related */
  unsigned long mm_usage;

  /* linked list */
  struct proc_info* next;
  struct proc_info* prev;
} proc_info_t;


typedef struct
{
  /* Start of the recording */
  ps_timestp_t tmstart;
  ps_timestp_t tmend;

  /* List of records */
  proc_info_t* head;

  /* Process related information */
  char* cmdline;
  unsigned long ps_retval;
  ps_id_t ps_id;
  ps_handle_t ps_hdl;
  
  FILE* outstrm;

  /* Flag */
  bool_t done;
  
} ps_mon_t;


#endif /* ! PSMON_H */
