//
// process.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Mon Oct 10 18:45:02 2005 texane
// Last update Sat Oct 22 03:32:35 2005 
//

#include <iostream>
#include <cstring>
#include <cstdio>
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


bool	win32::process::create_and_loadexec(handle_t* hdl, int ac, const char** av, const char** env)
{
  BOOL res;
  char* cmdline;
  PROCESS_INFORMATION psinfo;
  STARTUPINFO startinfo;

  ZeroMemory(&startinfo, sizeof(STARTUPINFO));
  startinfo.cb = sizeof(STARTUPINFO);

  cmdline = av_to_cmdline(ac, av);
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
    return false;

  *hdl = psinfo.hProcess;
  
  return true;
}


// Create a child process whose standart output
// will be accessible by reading the hdl_read handle
bool	win32::process::create_outredir_and_loadexec(handle_t* child_hdl,
						     win32::file::handle_t* read_hdl,
						     int ac,
						     const char** av,
						     const char** env)
{
  SECURITY_ATTRIBUTES sa;
  PROCESS_INFORMATION psinfo;
  STARTUPINFO startinfo;
  HANDLE write_hdl;
  HANDLE stdout_hdl;
  char* cmdline;

  // Create command line from av
  cmdline = av_to_cmdline(ac, av);

  // save the current stdout handle
  stdout_hdl = GetStdHandle(STD_OUTPUT_HANDLE);
  if (stdout_hdl == INVALID_HANDLE_VALUE)
    return false;

  // Create an anonymous pipe
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = NULL;
  if (CreatePipe(read_hdl, &write_hdl, &sa, 0) == FALSE)
    return false;
  SetHandleInformation(*read_hdl, HANDLE_FLAG_INHERIT, 0);

  // Set the write end of the pipe as the child stdout handle
  SetStdHandle(STD_OUTPUT_HANDLE, write_hdl);

  // Create a new process
  ZeroMemory(&psinfo, sizeof(psinfo));
  ZeroMemory(&startinfo, sizeof(startinfo));
  startinfo.cb = sizeof(startinfo);
  startinfo.hStdOutput = write_hdl;
  startinfo.dwFlags |= STARTF_USESTDHANDLES;
  if (CreateProcess(NULL, cmdline,
		    NULL, NULL,
		    TRUE,
		    CREATE_NO_WINDOW,
		    (LPVOID)env,
		    NULL,
		    &startinfo,
		    &psinfo) == FALSE)
    return false;

  delete[] cmdline;

  // Get the child handle
  *child_hdl = psinfo.hProcess;

  // Close the parent write part of the pipe
  CloseHandle(write_hdl);
  // Reset the parent stdout
  SetStdHandle(STD_OUTPUT_HANDLE, stdout_hdl);
  
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
