//
// file.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 17 18:10:19 2005 
// Last update Sat Oct 22 14:31:33 2005 
//


#include <posix.hh>


bool	posix::file::open(handle_t* hdl, const char* filename, mode_t mode, int* err)
{
  int fd;

  fd = ::open(filename, mode, 0);
  if (fd == -1)
    {
      return false;
    }

  *hdl = fd;
  return true;
}


bool	posix::file::create(const char* filename, mode_t mode, handle_t* res, int* err)
{
  int fd;

  fd = ::open(filename, mode);
  if (fd == -1)
    {
      return false;
    }

  // We want to keep the handle opened
  if (res)
    *res = fd;
  else
    ::close(fd);

  return true;
}

bool	posix::file::close(handle_t hdl, int*)
{
  ::close(hdl);
  return true;
}

bool	posix::file::read(handle_t hdl, unsigned char* buf, size_t sz_max, size_t* nr_read, int* err)
{
  ssize_t nr_bytes;

  nr_bytes = ::read(hdl, static_cast<void*>(buf), sz_max);
  if (nr_bytes == -1)
    {
      return false;
    }

  if (nr_read)
    {
      *nr_read = nr_bytes;
    }
  
  return true;
}

bool	posix::file::write(handle_t hdl, const unsigned char* buf, size_t sz_max, size_t* nr_written, int* err)
{
  ssize_t nr_bytes;

  nr_bytes = ::write(hdl, static_cast<const void*>(buf), sz_max);

  if (nr_bytes == -1)
    {
      return false;
    }

  if (nr_written)
    {
      *nr_written = nr_bytes;
    }

  return true;
}

bool	posix::file::seek(handle_t hdl, offset_t off, pos_t pos, int* err)
{
  off_t res;

  res = lseek(hdl, off, pos);
  
  if (res == -1)
    {
      return false;
    }

  return true;
}


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

  switch (q)
    {
    case EXISTS:
      if (ret != -1)
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

  return res;
}

bool	posix::file::exists(const char* filename)
{
  return file_query_about(filename, EXISTS, 0);
}

bool	posix::file::is_directory(const char* filename)
{
  return file_query_about(filename, DIRECTORY, 0);
}

bool	posix::file::is_readable(const char* filename)
{
  return file_query_about(filename, READABLE, 0);
}

bool	posix::file::is_writtable(const char* filename)
{
  return file_query_about(filename, WRITTABLE, 0);
}

bool	posix::file::is_executable(const char* filename)
{
  return file_query_about(filename, EXECUTABLE, 0);
}

bool	posix::file::size(const char* filename, unsigned long* sz)
{
  return file_query_about(filename, SIZE, sz);
}
