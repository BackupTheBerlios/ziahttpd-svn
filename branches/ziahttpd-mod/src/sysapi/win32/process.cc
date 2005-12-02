//
// process.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Mon Oct 10 18:45:02 2005 texane
// Last update Fri Dec 02 14:41:29 2005 texane
//


#include <iostream>
#include <cstring>
#include <cstdio>
#include <sysapi/win32.hh>
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

// Create a child process whose read and write std descriptors are redirected
// ! Close all handle on error
bool	win32::process::create_inoutredir_and_loadexec(handle_t* child_hdl,
						       win32::file::handle_t* hread,
						       win32::file::handle_t* hwrite,
						       int ac,
						       const char** av,
						       const char** env)
{
  SECURITY_ATTRIBUTES sa;
  PROCESS_INFORMATION psinfo;
  STARTUPINFO startinfo;
  HANDLE hwrite_pipe;
  HANDLE hread_pipe;
  HANDLE hstdout;
  HANDLE hstdin;
  char* cmdline;

  // Create command line from av
  cmdline = av_to_cmdline(ac, av);

  // save the current stdout handle
  hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hstdout == INVALID_HANDLE_VALUE)
    return false;

  hstdin = GetStdHandle(STD_INPUT_HANDLE);
  if (hstdin == INVALID_HANDLE_VALUE)
    {
      CloseHandle(hstdin);
      return false;
    }

  // Create an anonymous pipe
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = NULL;

  // Create the pipe child will read from
  if (CreatePipe(hread, &hwrite_pipe, &sa, 0) == FALSE)
    return false;
  SetHandleInformation(*hread, HANDLE_FLAG_INHERIT, 0);


  // Create the pipe child will write to
  if (CreatePipe(&hread_pipe, hwrite, &sa, 0) == FALSE)
    return false;
  SetHandleInformation(*hwrite, HANDLE_FLAG_INHERIT, 0);
  

  // Set child handles
  SetStdHandle(STD_OUTPUT_HANDLE, hwrite_pipe);
  SetStdHandle(STD_INPUT_HANDLE, hread_pipe);

  // Create a new process
  ZeroMemory(&psinfo, sizeof(psinfo));
  ZeroMemory(&startinfo, sizeof(startinfo));
  startinfo.cb = sizeof(startinfo);
  startinfo.hStdOutput = hwrite_pipe;
  startinfo.hStdInput = hread_pipe;
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
  CloseHandle(hwrite_pipe);
  CloseHandle(hread_pipe);
  // Reset the parent stdout
  SetStdHandle(STD_OUTPUT_HANDLE, hstdout);
  SetStdHandle(STD_INPUT_HANDLE, hstdin);
  
  return true;
}


bool	win32::process::myhandle(handle_t* myhdl)
{
  *myhdl = GetCurrentProcess();
  return true;
}


bool	win32::process::wait_single(handle_t hdl, state_t* st, waitopt_t wopt)
{
  DWORD ret;

  ret = WaitForSingleObject(hdl, (wopt == DONTWAIT) ? 0 : INFINITE);
  if (ret == WAIT_TIMEOUT)
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
