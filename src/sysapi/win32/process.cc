//
// process.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Mon Oct 10 18:45:02 2005 texane
// Last update Wed Oct 12 10:02:55 2005 texane
//

#include <cstring>
#include <win32.hh>
#include <windows.h>

// @see the win32.hh file for the namespace declaration


static char* av_to_cmdline(int ac, const char** av)
{
  char* cmdline;
  char* ptr;
  int nr_bytes;
  int i;

  nr_bytes = 0;
  for (i = 0; i < ac; ++i)
    {
      nr_bytes += (int)(strlen(av[i]) + 1);
    }

  ptr = cmdline = new char[nr_bytes + 1];
  i = 0;
  while (i < ac)
    {
      strcpy(ptr, av[i]);
      ptr += strlen(av[i]);
      if (++i < ac)
	{
	  *ptr = ' ';
	}
      else
	{
	  *ptr = 0;
	}
      ++ptr;
    }

  return cmdline;
}

#include <iostream>
bool	win32::process::create_and_loadexec(handle_t* hdl, int ac, const char** av, const char** env)
{
  BOOL res;
  char* cmdline;
  PROCESS_INFORMATION psinfo;
  STARTUPINFO startinfo;

  ZeroMemory(&startinfo, sizeof(STARTUPINFO));
  startinfo.cb = sizeof(STARTUPINFO);

  std::cout << "Command line is : " << ac << std::endl;
  cmdline = av_to_cmdline(ac, av);
  std::cout << "Command line is : " << cmdline << std::endl;
  res = CreateProcess(av[0], cmdline,
		      NULL, NULL,
		      true,
		      CREATE_NO_WINDOW,
		      reinterpret_cast<LPVOID>(env),
		      NULL,
		      &startinfo,
		      &psinfo);
  delete[] cmdline;
  if (res == FALSE)
    {
      return false;
    }

  *hdl = psinfo.hProcess;
  
  return true;
}


bool	win32::process::myhandle(handle_t* myhdl)
{
  *myhdl = GetCurrentProcess();
  return true;
}


bool	win32::process::wait_single(handle_t hdl, state_t* st, waitopt_t wopt)
{
  if (WaitForSingleObject(hdl, DONTWAIT ? 0 : INFINITE) == FALSE)
    return false;
  return true;
}


bool	win32::process::wait_any(handle_t* hdl, state_t* st, waitopt_t wopt)
{
  return false;
}


bool	win32::process::signal(handle_t hdl, sigid_t sig)
{
  bool res = true;

  switch (sig)
    {

    case TERMINATE:
      if (TerminateProcess(hdl, 0) == FALSE)
	res = false;
      break;

    default:
      break;
    }

  return res;
}


bool	win32::process::release(handle_t hdl)
{
  if (CloseHandle(hdl) == FALSE)
    return false;
  return true;
}
