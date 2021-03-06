//
// file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 14:10:39 2006 texane
// Last update Thu Feb 23 02:23:01 2006 texane
//


#include <string>
#include <sys/sysapi.hh>
#include <windows.h>


using std::string;


void sysapi::file::reset_handle(handle_t& file_handle)
{
  file_handle = INVALID_HANDLE_VALUE;
}


bool sysapi::file::handle_isset(handle_t& file_handle)
{
  return !(file_handle == INVALID_HANDLE_VALUE);
}


sysapi::error::handle_t sysapi::file::open(handle_t& hfile, const std::string& path, omode_t omode)
{
  sysapi::error::handle_t herr;
  DWORD shmode;
  DWORD op_mod;

  herr = error::SUCCESS;
  shmode = 0;
  op_mod = 0;
  if (omode & O_READ)
    {
      op_mod |= GENERIC_READ;
      shmode |= FILE_SHARE_READ;
    }
  if (omode & O_WRITE)
    {
      op_mod |= GENERIC_WRITE;
      shmode |= FILE_SHARE_WRITE;
    }
  hfile = CreateFile(path.c_str(), op_mod, shmode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (hfile == INVALID_HANDLE_VALUE)
    herr = error::OPEN_FAILED;
    // herr = GetLastError();
  return herr;
}


sysapi::error::handle_t sysapi::file::close(handle_t& hfile)
{
  CloseHandle(hfile);
// +  reset_handle(hfile);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::read(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nread)
{
  DWORD nr_bytes;
  BOOL ret;

  nread = 0;
  ret = ReadFile(hfile, static_cast<LPVOID>(buf), nbytes, &nr_bytes, NULL);
  if (ret == FALSE)
    return error::READ_FAILED;
  nread = (unsigned int)nr_bytes;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::read_nonblock(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nread)
{
  unsigned char one_buf;
  DWORD nr_bytes;
  DWORD nr_avail;
  BOOL ret;

  // are there data pending in the pipe
  ret = PeekNamedPipe(hfile, (LPVOID)&one_buf, sizeof(unsigned char), &nr_bytes, &nr_avail, 0);
  if (ret == FALSE)
    {
      return sysapi::error::READ_FAILED;
    }
  if (nr_bytes == 0)
    {
      // no data to read
      nread = 0;
      return sysapi::error::OPERATION_WOULDBLOCK;
    }
  return sysapi::file::read(hfile, buf, nbytes, nread);
}


sysapi::error::handle_t sysapi::file::write(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nwritten)
{
  DWORD nr_bytes;
  BOOL ret;

  ret = WriteFile(hfile, reinterpret_cast<LPVOID>((void*)buf), nbytes, &nr_bytes, NULL);
  if (ret == FALSE)
    return error::WRITE_FAILED;

  nwritten = nr_bytes;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::size(handle_t& hfile, unsigned long long& sz)
{
  BY_HANDLE_FILE_INFORMATION info;

  if (GetFileInformationByHandle(hfile, &info) == FALSE)
    return error::UNKNOWN;
  sz = info.nFileSizeLow;
  return error::SUCCESS;
}



// Query about api


// Query informations about a given file

enum file_query
  {
    DOES_EXIST = 0,
    GET_SIZE,
    IS_DIRECTORY,
    IS_READABLE,
    IS_WRITTABLE,
    IS_EXECUTABLE
  };

static bool normalfile_query_about(const char* filename, enum file_query q, unsigned long* aux)
{
  HANDLE hfile;
  BY_HANDLE_FILE_INFORMATION info;
  bool ret;

  ret = false;
  hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);
  if (hfile == INVALID_HANDLE_VALUE)
    {
      return false;
    }
  if (GetFileInformationByHandle(hfile, &info) == FALSE)
    {
      CloseHandle(hfile);
      return false;
    }

  switch (q)
    {
    case DOES_EXIST:
      ret = true;
      break;

    case IS_DIRECTORY:
      if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	ret = true;
      break;

    case GET_SIZE:
      *aux = (unsigned long)info.nFileSizeLow;
      ret = true;
      break;

    case IS_READABLE:
      ret = true;
      break;

    case IS_WRITTABLE:
      ret = true;
      break;

    case IS_EXECUTABLE:
      ret = true;
      break;

    default:
      ret = false;
      break;      
    }

  CloseHandle(hfile);

  return ret;
}

static bool directory_query_about(const char* filename, enum file_query q, unsigned long* aux)
{
  return true;
}

static bool file_query_about(const char* filename, enum file_query q, unsigned long* aux)
{
  return normalfile_query_about(filename, q, aux);
}

bool sysapi::file::is_path_valid(const string& filename)
{
  return file_query_about(filename.c_str(), DOES_EXIST, 0);
}

bool sysapi::file::is_directory(const string& filename)
{
  return file_query_about(filename.c_str(), IS_DIRECTORY, 0);
}

bool sysapi::file::is_readable(const string& filename)
{
  return file_query_about(filename.c_str(), IS_READABLE, 0);
}

bool sysapi::file::is_writable(const string& filename)
{
  return file_query_about(filename.c_str(), IS_WRITTABLE, 0);
}

bool sysapi::file::is_executable(const string& filename)
{
  return file_query_about(filename.c_str(), IS_EXECUTABLE, 0);
}
