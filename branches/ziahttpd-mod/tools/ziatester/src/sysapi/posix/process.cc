//
// process.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:32:20 2005 
// Last update Sun Nov 13 15:00:43 2005 
//


#include <sysapi/posix.hh>


bool	posix::process::create_and_loadexec(handle_t* child_hdl, int ac, const char** av, const char** env)
{
  // Create a child process
  *child_hdl = fork();
  if (*child_hdl == -1)
    return false;

  // We are in the child
  if (*child_hdl == 0)
    {
      execve(*av, (char* const*)av, (char* const*)env);
      exit(-1);
    }

  return true;
}


bool	posix::process::create_outredir_and_loadexec(handle_t* child_hdl, posix::file::handle_t* read_hdl, int ac, const char** av, const char** env)
{
  int fds[2];

  // Create a pipe
  if (pipe(fds) == -1)
    return false;

  // Create a child process
  if ((*child_hdl = fork()) == -1)
    return false;

  // We are in the child
  if (*child_hdl == 0)
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
  *read_hdl = fds[0];

  return true;
}


bool	posix::process::create_inoutredir_and_loadexec(handle_t* hchild, posix::file::handle_t* hread, posix::file::handle_t* hwrite, int ac, const char** av, const char** env)
{
  int fds[2];

  // Create a bidirectional communication channel
  if (socketpair(AF_UNIX, SOCK_STREAM, PF_UNSPEC, fds) == -1)
    return false;

  // Create a child process
  if ((*hchild = fork()) == -1)
    return false;

  // We are in the child
  if (*hchild == 0)
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
  *hread = fds[1];
  *hwrite = fds[1];

  return true;
}


bool	posix::process::myhandle(handle_t* hdl)
{
  *hdl = getpid();
  return true;
}


bool	posix::process::signal(handle_t, sigid_t)
{
  return false;
}


bool	posix::process::release(handle_t)
{
  // always return true
  return true;
}


// !fixme: fill in the state
bool	posix::process::wait_single(handle_t hdl, state_t* , waitopt_t wopt)
{
  int opt = 0;
  int stat;

  if (wopt == DONTWAIT)
    opt = WNOHANG;

  if (waitpid(hdl, &stat, opt) == -1)
    return false;

  return true;
}

// !fixme: fill in the state
bool	posix::process::wait_any(handle_t* hdl, state_t*, waitopt_t wopt)
{
  // SHOULD NOT BE PART OF THE INTERFACE, THE PROCESS HAS TO KEEP TRACK OF CHILDREN AND WAIT_SINGLE
  return false;
}
