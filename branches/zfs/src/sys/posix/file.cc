//
// file.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:01:40 2006 
// Last update Wed Feb 22 20:46:33 2006 texane
//

#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/sysapi.hh>


using std::string;


void sysapi::file::reset_handle(handle_t& file_handle)
{
  file_handle = -1;
}


bool sysapi::file::handle_isset(handle_t& file_handle)
{
  return !(file_handle == -1);
}


sysapi::error::handle_t sysapi::file::open(handle_t& hfile, const std::string& path, omode_t omode)
{
  int fd;
  int op_mod;

  hfile = -1;
  op_mod = 0;
  if (omode == O_BOTH)
    op_mod = O_RDWR;
  else if (omode == O_READ)
    op_mod = O_RDONLY;
  else if (omode == O_WRITE)
    op_mod = O_WRONLY;
  fd = ::open(path.c_str(), op_mod);
  if (fd == -1)
    return error::OPEN_FAILED;
  hfile = fd;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::close(handle_t& hfile)
{
  ::close(hfile);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::read(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nread)
{
  int nret;

  nret = ::read(hfile, buf, nbytes);
  if (nret <= 0)
    return error::READ_FAILED;
  nread = (unsigned int)nret;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::read_nonblock(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nread)
{
  sysapi::error::handle_t sys_err;
  int nret;

  fcntl(hfile, F_SETFL, O_NONBLOCK);
  errno = 0;
  nret = ::read(hfile, nbytes, nread);
  if (nret == -1 && errno == EAGAIN)
    {
      // would block
      nread = 0;
      sys_err = sysapi::error::OPERATION_WOULDBLOCK;
    }
  else if (nret <= 0)
    {
      // error
      nread = 0;
      sys_err = error::READ_FAILED;
    }
  else
    {
      // success
      sys_err = error::SUCCESS;
    }
  fcntl(hfile, F_SETFL, ~O_NONBLOCK);
  return sys_err;
}


sysapi::error::handle_t sysapi::file::write(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nwritten)
{
  int nret;

  nret = ::write(hfile, buf, nbytes);
  if (nret <= 0)
    return error::WRITE_FAILED;
  nwritten = (unsigned int)nret;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::size(handle_t& hfile, unsigned long long& sz)
{
  struct stat st;

  if (fstat(hfile, &st) == -1)
    return error::UNKNOWN;
  sz = (unsigned long long)st.st_size;
  return error::SUCCESS;
}


// Query about api


enum file_query
  {
    EXISTS = 0,
    DIRECTORY,
    SIZE,
    READABLE,
    WRITTABLE,
    EXECUTABLE
  };

static bool file_query_about(const char* filename, enum file_query q, unsigned long* aux)
{
  struct stat st;
  int ret;
  bool res;

  res = false;
  ret = stat(filename, &st);
  if (ret != -1)
    {
      switch (q)
	{
	case EXISTS:
	  res = true;
	  break;

	case DIRECTORY:
	  if ((st.st_mode & S_IFMT) == S_IFDIR)
	    res = true;
	  break;

	case SIZE:
	  *aux = (unsigned long)st.st_size;
	  res = true;
	  break;

	case READABLE:
	  res = true;
	  break;

	case WRITTABLE:
	  res = true;
	  break;

	case EXECUTABLE:
	  res = true;
	  break;

	default:
	  break;
	}
    }

  return res;
}

bool sysapi::file::is_path_valid(const string& filename)
{
  return file_query_about(filename.c_str(), EXISTS, 0);
}

bool sysapi::file::is_directory(const string& filename)
{
  return file_query_about(filename.c_str(), DIRECTORY, 0);
}

bool sysapi::file::is_readable(const string& filename)
{
  return file_query_about(filename.c_str(), READABLE, 0);
}

bool sysapi::file::is_writable(const string& filename)
{
  return file_query_about(filename.c_str(), WRITTABLE, 0);
}

bool sysapi::file::is_executable(const string& filename)
{
  return file_query_about(filename.c_str(), EXECUTABLE, 0);
}
