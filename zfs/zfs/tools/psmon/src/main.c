/*
** main.c for  in 
** 
** Made by texane
** Login   <texane@gmail.com>
** 
** Started on  Sat Jan 28 20:52:31 2006 texane
** Last update Sun Jan 29 01:00:25 2006 texane
*/


#include <windows.h>
#include <psapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <psmon.h>


static void proc_info_push_back(proc_info_t** head, proc_info_t* tail)
{
  proc_info_t* pi;

  if (*head == 0)
    *head = tail;
  else
    {
      for (pi = *head; pi->next; pi = pi->next)
	;
      tail->prev = pi;
      pi->next = tail;
    }
}


static void proc_info_reset(proc_info_t* pi)
{
  pi->tmstp = 0;
  pi->proc_usage = 0;
  pi->mm_usage = 0;
  pi->tmstp = 0;
  pi->next = 0;
  pi->prev = 0;
}

static proc_info_t* proc_info_alloc(void)
{
  proc_info_t* pi;

  pi = malloc(sizeof(proc_info_t));
  if (pi)
    proc_info_reset(pi);
  return pi;
}

static void proc_info_release(proc_info_t* pi)
{
  free(pi);
}

static void get_cpu_usage(ps_mon_t* psmon, proc_info_t* pi, proc_info_t* prev)
{
  ULARGE_INTEGER uli1;
  ULARGE_INTEGER uli2;
  ULARGE_INTEGER uli1_tot;
  ULARGE_INTEGER uli2_tot;
  FILETIME tm_create;
  FILETIME tm_exit;
  FILETIME tm_kernel;
  FILETIME tm_user;

  SYSTEMTIME tm_current;
  FILETIME tm_old_time;
  FILETIME tm_cur_time;
  FILETIME tm_delta_consumed;

  BOOL ret;

  ret = GetProcessTimes(psmon->ps_handle,
			&tm_create,
			&tm_exit,
			&tm_kernel,
			&tm_user);
  if (ret == FALSE)
    return ;

  memcpy(&uli1, &tm_kernel, sizeof(ULARGE_INTEGER));
  memcpy(&uli2, &tm_user, sizeof(ULARGE_INTEGER));
  uli2.QuadPart += uli1.QuadPart;

  GetSystemTime(&tm_current);
  SystemToFileTime(&tm_current, &tm_cur_time);
  memcpy(&uli1_tot, &tm_cur_time, sizeof(ULARGE_INTEGER));
  memcpy(&uli2_tot, &tm_old_time, sizeof(ULARGE_INTEGER));
  uli1_tot.QuadPart -= uli2_tot.QuadPart;
  uli1_tot.QuadPart /= uli2.QuadPart;
  
}

static ps_timestp_t time_from_epoch(void)
{
  SYSTEMTIME stm;
  FILETIME fltm;
  ULARGE_INTEGER uli1;
  ULARGE_INTEGER uli2;
  unsigned long long nsec;

  GetSystemTime(&stm);
  SystemTimeToFileTime(&stm, &fltm);
  memcpy(&uli1, &fltm, sizeof(uli1));

  memset(&stm, 0, sizeof(SYSTEMTIME));
  stm.wYear = 1970;
  stm.wMonth = 1;
  stm.wDay = 1;
  SystemTimeToFileTime(&stm, &fltm);
  memcpy(&uli2, &fltm, sizeof(uli2));
  nsec = (uli1.QuadPart - uli2.QuadPart) / 10000000;

  return nsec;
}

static void proc_info_report(ps_mon_t* psmon, proc_info_t* pi)
{
  fprintf(psmon->outstrm, "<tr><td>memory</td><td>%8ld</td></tr>\n", pi->mm_usage / 1024);
  fprintf(psmon->outstrm, "<tr><td>processor</td><td>%8ld</td></tr>\n", pi->cpu_usage);

/*   printf("%lu\n", nsec); */

/*   printf("[%08ld]", pi->tmstp); */
/*   printf("{\n"); */
/*   printf("  mem == %08ld\n", pi->mm_usage); */
/*   if (pi->prev) */
/*     { */
/*       mm_var = pi->mm_usage - pi->prev->mm_usage; */
/*       printf("memory variation: %d\n", mm_var); */
/*     } */
/*   printf("};\n"); */
/*   fflush(stdout); */
}


static int ps_monitor(ps_mon_t* psmon)
{
  PROCESS_MEMORY_COUNTERS mem_counters;
  proc_info_t* pi;

  pi = proc_info_alloc();
  pi->tmstp = time_from_epoch();
  if (GetProcessMemoryInfo(psmon->ps_hdl, &mem_counters, sizeof(PROCESS_MEMORY_COUNTERS)) != 0)
    pi->mm_usage = (unsigned long)mem_counters.WorkingSetSize;
  else
    pi->mm_usage = 0;
  proc_info_push_back(&psmon->head, pi);
  proc_info_report(psmon, pi);
  return 0;
}


/* /\* @see function GetProcessTimes() *\/ */

/* static void ps_report(proc_info_t* pi) */
/* { */
/*   while (pi) */
/*     { */
/*       proc_info_report(pi); */
/*       pi = pi->next; */
/*     } */
/* } */


static bool_t ps_mon_init(ps_mon_t* psmon, char* cmdline)
{
  BOOL ret;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  memset(&si, 0, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  ret = CreateProcess(0, (LPTSTR)cmdline,
		      0, 0, FALSE,
		      0,
		      NULL,
		      0,
		      &si,
		      &pi);

  psmon->tmstart = time_from_epoch();
  psmon->tmend = psmon->tmend;
  if (ret == FALSE)
    {
      psmon->done = true;
      printf("cannot create the process\n");
      fflush(stdout);
      return false;
    }

  psmon->head = 0;
  psmon->done = false;
  psmon->ps_id = pi.dwProcessId;
  psmon->ps_hdl = pi.hProcess;
  psmon->cmdline = cmdline;
  psmon->outstrm = fopen("out.html", "w");
  if (psmon->outstrm == NULL)
    {
      printf("Cannot open\n");
      fflush(stdout);
      psmon->done = true;
      return false;
    }
  fprintf(psmon->outstrm, "<html><body>\n");
  fprintf(psmon->outstrm, "<table>\n");
  fprintf(psmon->outstrm, "<tr><td>%u</td><td>%s</td></tr>\n", psmon->ps_id, psmon->cmdline);
  fprintf(psmon->outstrm, "<tr><td>time</td><td>%ul</td></tr>\n", psmon->tmstart);

  return true;
}

static bool_t ps_mon_release(ps_mon_t* psmon)
{
  fprintf(psmon->outstrm, "<tr><td>retval</td><td>%u</td></tr>\n", psmon->ps_retval);
  fprintf(psmon->outstrm, "<tr><td>time</td><td>%ul</td></tr>\n", psmon->tmend);
  fprintf(psmon->outstrm, "</table>\n");
  fprintf(psmon->outstrm, "</body></html>");
  fclose(psmon->outstrm);
  CloseHandle(psmon->outstrm);
  CloseHandle(psmon->ps_hdl);
  return true;
}

static bool_t ps_mon_waitfor(ps_mon_t* psmon)
{
  DWORD ret;

  while (psmon->done == false)
    {
      ret = WaitForSingleObject(psmon->ps_hdl, 500);
      ps_monitor(psmon);
      if (ret != WAIT_TIMEOUT)
	{
	  psmon->tmend = time_from_epoch();
	  GetExitCodeProcess(psmon->ps_hdl, &psmon->ps_retval);
	  psmon->done = true;
	}
    }
  return true;
}


int main(int ac, char** av)
{
  ps_mon_t psmon;

  ps_mon_init(&psmon, av[1]);
  ps_mon_waitfor(&psmon);
  ps_mon_release(&psmon);
  return 0;
}
