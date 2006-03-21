//
// process.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Feb 18 10:47:48 2006 texane
// Last update Sat Feb 18 23:42:19 2006 texane
//


#include <sys/sysapi.hh>


// helpers

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


// exported

void sysapi::process::reset_handle(handle_t& proc_handle)
{
  proc_handle = INVALID_HANDLE_VALUE;
}


bool sysapi::process::handle_isset(handle_t& proc_handle)
{
  return !(proc_handle == INVALID_HANDLE_VALUE);
}


sysapi::error::handle_t sysapi::process::create_and_loadexec(handle_t& hdl, int ac, const char** av, const char** env)
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
    return error::OPEN_FAILED;
  hdl = psinfo.hProcess;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::create_outredir_and_loadexec(handle_t& child_hdl, sysapi::file::handle_t& read_hdl, int ac, const char** av, const char** env)
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
    return error::OPEN_FAILED;

  // Create an anonymous pipe
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = NULL;
  if (CreatePipe(&read_hdl, &write_hdl, &sa, 0) == FALSE)
    return error::OPEN_FAILED;
  SetHandleInformation(read_hdl, HANDLE_FLAG_INHERIT, 0);

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
    return error::OPEN_FAILED;

  delete[] cmdline;

  // Get the child handle
  child_hdl = psinfo.hProcess;

  // Close the parent write part of the pipe
  CloseHandle(write_hdl);
// +  reset_handle(write_hdl);
  // Reset the parent stdout
  SetStdHandle(STD_OUTPUT_HANDLE, stdout_hdl);

  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::create_inoutredir_and_loadexec(handle_t& child_hdl, sysapi::file::handle_t& hread, sysapi::file::handle_t& hwrite, int ac, const char** av, const char** env)
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
    return error::OPEN_FAILED;

  hstdin = GetStdHandle(STD_INPUT_HANDLE);
  if (hstdin == INVALID_HANDLE_VALUE)
    {
      CloseHandle(hstdin);
// + reset_handle(hstdin);
      return error::OPEN_FAILED;
    }

  // Create an anonymous pipe
  sa.nLength = sizeof(sa);
  sa.bInheritHandle = TRUE;
  sa.lpSecurityDescriptor = NULL;

  // Create the pipe child will read from
  if (CreatePipe(&hread, &hwrite_pipe, &sa, 0) == FALSE)
    return error::OPEN_FAILED;
  SetHandleInformation(hread, HANDLE_FLAG_INHERIT, 0);


  // Create the pipe child will write to
  if (CreatePipe(&hread_pipe, &hwrite, &sa, 0) == FALSE)
    return error::OPEN_FAILED;
  SetHandleInformation(hwrite, HANDLE_FLAG_INHERIT, 0);
  

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
    return error::OPEN_FAILED;

  delete[] cmdline;

  // Get the child handle
  child_hdl = psinfo.hProcess;

  // Close the parent write part of the pipe
  CloseHandle(hwrite_pipe);
  CloseHandle(hread_pipe);
  // Reset the parent stdout
  SetStdHandle(STD_OUTPUT_HANDLE, hstdout);
  SetStdHandle(STD_INPUT_HANDLE, hstdin);
  
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::wait_single(handle_t& hdl, state_t& st, waitopt_t wopt)
{
  DWORD ret;

  ret = WaitForSingleObject(hdl, (wopt == DONTWAIT) ? 0 : INFINITE);
  if (ret == WAIT_TIMEOUT)
    return error::OPERATION_TIMEOUTED;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::terminate(handle_t& hdl)
{
  if (TerminateProcess(hdl, 0) == FALSE)
    return error::UNKNOWN;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::release(handle_t& proc_handle)
{
  terminate(proc_handle);
  CloseHandle(proc_handle);
  return error::SUCCESS;
}
