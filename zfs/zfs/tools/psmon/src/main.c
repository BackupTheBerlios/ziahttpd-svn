/*
** main.c for  in 
** 
** Made by texane
** Login   <texane@gmail.com>
** 
** Started on  Sat Jan 28 20:52:31 2006 texane
** Last update Wed Feb 01 04:03:23 2006 texane
*/


#define _WIN32_WINNT 0x0501
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
  pi->cpu_usage = 0;
  pi->kcpu_usage = 0;
  pi->ucpu_usage = 0;
  pi->mm_usage = 0;
  pi->hdl_count = 0;
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

static void get_cpu_usage(ps_mon_t* psmon, proc_info_t* pi)
{
  ULARGE_INTEGER cpu_usage;
  ULARGE_INTEGER kcpu_usage;
  ULARGE_INTEGER ucpu_usage;
  ULARGE_INTEGER old_cpu_usage;
  ULARGE_INTEGER old_kcpu_usage;
  ULARGE_INTEGER old_ucpu_usage;
  ULARGE_INTEGER delta_cpu_usage;
  ULARGE_INTEGER delta_kcpu_usage;
  ULARGE_INTEGER delta_ucpu_usage;
  ULARGE_INTEGER current_time;
  ULARGE_INTEGER old_time;
  ULARGE_INTEGER delta_time;
  ULARGE_INTEGER tmp;
  SYSTEMTIME tm_current;
  FILETIME tm_cur_time;
  FILETIME tm_create;
  FILETIME tm_exit;
  FILETIME tm_kernel;
  FILETIME tm_user;
  BOOL ret;

  ret = GetProcessTimes(psmon->ps_hdl,
			&tm_create,
			&tm_exit,
			&tm_kernel,
			&tm_user);
  if (ret == FALSE)
    return ;


  /* Get the total cpu time consumed until now */
  cpu_usage.QuadPart = 0;
  memcpy(&tmp, &tm_kernel, sizeof(ULARGE_INTEGER));
  cpu_usage.QuadPart += tmp.QuadPart;
  memcpy(&tmp, &tm_user, sizeof(ULARGE_INTEGER));
  cpu_usage.QuadPart += tmp.QuadPart;

  kcpu_usage.QuadPart = 0;
  memcpy(&tmp, &tm_kernel, sizeof(ULARGE_INTEGER));
  kcpu_usage.QuadPart += tmp.QuadPart;

  ucpu_usage.QuadPart = 0;
  memcpy(&tmp, &tm_user, sizeof(ULARGE_INTEGER));
  ucpu_usage.QuadPart += tmp.QuadPart;

  /* Get the current time */
  GetSystemTime(&tm_current);
  SystemTimeToFileTime(&tm_current, &tm_cur_time);
  memcpy(&current_time, &tm_cur_time, sizeof(FILETIME));

  /* This is the first time we come here */
  if (psmon->tm_last_time.QuadPart == 0)
    {
      psmon->tm_cpu_usage.QuadPart = cpu_usage.QuadPart;
      psmon->tm_ucpu_usage.QuadPart = ucpu_usage.QuadPart;
      psmon->tm_kcpu_usage.QuadPart = kcpu_usage.QuadPart;
      psmon->tm_last_time.QuadPart = current_time.QuadPart;
      psmon->load_cpu = 0;
      return ;
    }

  /* Save the current time for later use and compute delta */
  old_time.QuadPart = psmon->tm_last_time.QuadPart;
  psmon->tm_last_time.QuadPart = current_time.QuadPart;
  delta_time.QuadPart = current_time.QuadPart - old_time.QuadPart;

  /* Save the current cpu and compute delta */
  old_cpu_usage.QuadPart = psmon->tm_cpu_usage.QuadPart;
  old_kcpu_usage.QuadPart = psmon->tm_kcpu_usage.QuadPart;
  old_ucpu_usage.QuadPart = psmon->tm_ucpu_usage.QuadPart;
  psmon->tm_cpu_usage.QuadPart = cpu_usage.QuadPart;
  psmon->tm_ucpu_usage.QuadPart = ucpu_usage.QuadPart;
  psmon->tm_kcpu_usage.QuadPart = kcpu_usage.QuadPart;
  delta_cpu_usage.QuadPart = cpu_usage.QuadPart - old_cpu_usage.QuadPart;
  delta_ucpu_usage.QuadPart = ucpu_usage.QuadPart - old_ucpu_usage.QuadPart;
  delta_kcpu_usage.QuadPart = kcpu_usage.QuadPart - old_kcpu_usage.QuadPart;
  if (delta_cpu_usage.QuadPart == 0)
    {
      psmon->load_cpu = 0;
    }
  else
    {
      double res;
      res = (double)delta_cpu_usage.LowPart / (double)delta_time.LowPart;
      res *= 100;
      psmon->load_cpu = res;
      pi->cpu_usage = res;

      res = (double)delta_kcpu_usage.LowPart / (double)delta_time.LowPart;
      res *= 100;
      pi->kcpu_usage = res;

      res = (double)delta_ucpu_usage.LowPart / (double)delta_time.LowPart;
      res *= 100;
      pi->ucpu_usage = res;
/*       printf("delta cpu usage: %d\n", delta_cpu_usage.LowPart); */
/*       printf("delta time usage: %d\n", delta_time.LowPart); */
    }
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

static unsigned long color_from_delta(unsigned long load)
{
  unsigned long res = 0xffffff;
  unsigned short tmp;
  unsigned char n;
  n = (unsigned char)((0xff * load) / 100) & 0xf0;
  tmp = n + (n << 8);
  return res - tmp;
}

/* Here the base is 28, as a hint... max should be 128, but... */
static unsigned long color_from_handle_delta(unsigned long nhandle)
{
  unsigned long res = 0xffffff;
  unsigned short tmp;
  unsigned char n;

  /* normalize */
  nhandle -= 28;
  n = (unsigned char)((0xff * nhandle) / 3000) & 0xf0;
  tmp = n + (n << 8);
  return res - tmp;
}

static void proc_info_report(ps_mon_t* psmon, proc_info_t* pi)
{
/*   fprintf(psmon->outstrm, "<table frame=\"box\" border=\"1\" align=right">\n"); */
  fprintf(psmon->outstrm, "<tr>\n");
  fprintf(psmon->outstrm, "<td align=\"right\">%8lu</td>\n", pi->tmstp);
  fprintf(psmon->outstrm, "<td align=\"right\">%8ldKb</td>\n", pi->mm_usage / 1024);
  fprintf(psmon->outstrm, "<td align=\"right\" bgcolor=\"#%06x\">%8ld</td>\n", color_from_handle_delta(pi->hdl_count), pi->hdl_count);
  fprintf(psmon->outstrm, "<td align=\"right\" bgcolor=\"#%06x\">%.2lf%%</td>\n", color_from_delta((unsigned long)pi->cpu_usage), pi->cpu_usage);
  fprintf(psmon->outstrm, "<td align=\"right\" bgcolor=\"#%06x\">%.2lf%%</td>\n", color_from_delta((unsigned long)pi->kcpu_usage), pi->kcpu_usage);
  fprintf(psmon->outstrm, "<td align=\"right\" bgcolor=\"#%06x\">%.2lf%%</td>\n", color_from_delta((unsigned long)pi->ucpu_usage), pi->ucpu_usage);
  fprintf(psmon->outstrm, "</tr>\n");
/*   fprintf(psmon->outstrm, "</table>\n"); */

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


static void get_handle_count(ps_mon_t* psmon, proc_info_t* pi)
{
  if (GetProcessHandleCount(psmon->ps_hdl, (PDWORD)&pi->hdl_count) == FALSE)
    pi->hdl_count = 0;
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
  get_cpu_usage(psmon, pi);
  get_handle_count(psmon, pi);
/*   proc_info_push_back(&psmon->head, pi); */
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

  psmon->tm_last_time.QuadPart = 0;
  psmon->tm_cpu_usage.QuadPart = 0;
  psmon->tm_ucpu_usage.QuadPart = 0;
  psmon->tm_kcpu_usage.QuadPart = 0;
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
  fprintf(psmon->outstrm, "/* Report generated for (%lu, \"%s\")</br>", psmon->ps_id, psmon->cmdline);
  fprintf(psmon->outstrm, " * Started at time %lu</br>", psmon->tmstart);
  fprintf(psmon->outstrm, " */</br>");
  fprintf(psmon->outstrm, "<table frame=\"box\">\n");
  fprintf(psmon->outstrm, "<tr><th>Timestamp</th><th>MmWrkSet</th><th>HandleCount</th><th>CpuLoad</th><th>KernelLoad</th><th>UserLoad</th></tr>");

  return true;
}

static bool_t ps_mon_release(ps_mon_t* psmon)
{ 
  fprintf(psmon->outstrm, "</table>\n");
  fprintf(psmon->outstrm, "/* Done at time %lu</br>\n", psmon->tmend);
  fprintf(psmon->outstrm, " * Process exit code: %d</br>\n", psmon->ps_retval);
  fprintf(psmon->outstrm, " */</br>\n");
  fprintf(psmon->outstrm, "</body></html>\n");
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
      ret = WaitForSingleObject(psmon->ps_hdl, 50);
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
