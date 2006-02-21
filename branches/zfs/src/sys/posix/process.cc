//
// process.cc<3> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Feb 18 10:56:19 2006 texane
// Last update Tue Feb 21 23:26:14 2006 
//


#include <signal.h>
#include <sys/sysapi.hh>


void sysapi::process::reset_handle(handle_t& proc_handle)
{
  proc_handle = -1;
}


bool sysapi::process::handle_isset(handle_t& proc_handle)
{
  return !(proc_handle == -1);
}


sysapi::error::handle_t sysapi::process::create_and_loadexec(handle_t& child_hdl, int ac, const char** av, const char** env)
{
  // Create a child process
  child_hdl = fork();
  if (child_hdl == -1)
    return error::OPEN_FAILED;

  // We are in the child
  if (child_hdl == 0)
    {
      execve(*av, (char* const*)av, (char* const*)env);
      exit(-1);
    }

  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::create_outredir_and_loadexec(handle_t& child_hdl, sysapi::file::handle_t& read_hdl, int ac, const char** av, const char** env)
{
  int fds[2];

  // Create a pipe
  if (pipe(fds) == -1)
    return error::OPEN_FAILED;

  // Create a child process
  if ((child_hdl = fork()) == -1)
    return error::OPEN_FAILED;

  // We are in the child
  if (child_hdl == 0)
    {
      // close the read end of the pipe
      close(fds[0]);
      // stdout -> write end of the pipe
      if (dup2(fds[1], 1) == -1)
	exit(-1);
      
      execve(*av, (char* const*)av, (char* const*)env);
      exit(-1);
    }

  // close the write end
  close(fds[1]);
  read_hdl = fds[0];

  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::create_inoutredir_and_loadexec(handle_t& child_hdl, sysapi::file::handle_t& hread, sysapi::file::handle_t& hwrite, int ac, const char** av, const char** env)
{
  int fds[2];

  // Create a bidirectional communication channel
  if (socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, fds) == -1)
    return error::OPEN_FAILED;

  // Create a child process
  if ((child_hdl = fork()) == -1)
    return error::OPEN_FAILED;

  // We are in the child
  if (child_hdl == 0)
    {
      close(fds[1]);
      if (dup2(fds[0], 1) == -1)
	exit(-1);
      if (dup2(fds[0], 0) == -1)
	exit(-1);
      execve(*av, (char* const*)av, (char* const*)env);
      exit(-1);
    }

  // close the write end
  close(fds[0]);
  hread = fds[1];
  hwrite = fds[1];

  return error::SUCCESS;
}



sysapi::error::handle_t sysapi::process::wait_single(handle_t& hdl, state_t& st, waitopt_t wopt)
{
  int opt;
  int stat;

  opt = 0;
  if (wopt == DONTWAIT)
    opt = WNOHANG;
  if (waitpid(hdl, &stat, opt) == -1)
    return error::UNKNOWN;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::terminate(handle_t& proc_handle)
{
  kill(proc_handle, SIGKILL);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::process::release(handle_t& proc_handle)
{
  terminate(proc_handle);
  return error::SUCCESS;
}
