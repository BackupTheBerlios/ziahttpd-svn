//
// file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Mon Oct 10 12:51:47 2005 texane
// Last update Wed Nov 23 21:54:10 2005 texane
//


#include <sysapi/win32.hh>
#include <windows.h>
#include <iostream>


// @see win32.hh for the namespace declaration
// !fix: error code are not yet handled, we first have to implement errno like


// Open filename in the mode mode
bool	win32::file::open(win32::file::handle_t* handle,
			  const char* filename,
			  win32::file::mode_t mode,
			  int* err)
{
  *handle = CreateFile(filename, mode, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (*handle == INVALID_HANDLE_VALUE)
    {
      return false;
    }

  return true;
}


// Create a file and return a handle if specified
bool	win32::file::create(const char* filename,
			    mode_t mode,
			    handle_t* handle,
			    int* err)
{
  handle_t res;

  res = CreateFile(filename, mode, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  if (res == INVALID_HANDLE_VALUE)
    {
      return false;
    }
  // We want to keep the handle opened
  else if (handle)
    {
      *handle = res;
    }
  // We don't bother with the handle
  else
    {
      CloseHandle(res);
    }

  return true;
}


// Close the file pointed to by handle
bool	win32::file::close(win32::file::handle_t handle,
			   int* err)
{
  BOOL res;

  res = CloseHandle(handle);
  if (res)
    {
      return true;
    }

  return false;
}

bool	win32::file::close_rd(win32::file::handle_t hread,
			      int* err)
{
  return close(hread, err);
}


bool	win32::file::close_wr(win32::file::handle_t hwrite,
			      int* err)
{
  return close(hwrite, err);
}


// Read sz bytes to buf from offset from in file pointed to by handle
bool	win32::file::read(win32::file::handle_t handle,
			  unsigned char* buf,
			  win32::file::size_t sz,
			  size_t* len,
			  int* err)
{
  DWORD nr_bytes;
  BOOL ret;

  nr_bytes = 0;
  ret = ReadFile(handle, static_cast<LPVOID>(buf), sz, &nr_bytes, NULL);
  if (ret == FALSE)
    {
      return false;
    }

  if (len)
    {
      *len = nr_bytes;
    }
  return true;
}


// Write
bool	win32::file::write(win32::file::handle_t handle,
			   const unsigned char* buf,
			   win32::file::size_t sz,
			   size_t* len,
			   int* err)
{
  DWORD nr_bytes;
  BOOL ret;

  ret = WriteFile(handle, reinterpret_cast<LPVOID>((void*)buf), sz, &nr_bytes, NULL);
  if (ret == FALSE)
    {
      return false;
    }

  if (len)
    {
      *len = nr_bytes;
    }
  return true;
}


// Rewind seek associated to the file
bool	win32::file::seek(win32::file::handle_t handle,
			  win32::file::offset_t off,
			  win32::file::pos_t from,
			  int* err)
{
  DWORD res;

  res = SetFilePointer(handle, off, NULL, from);
  if (res == INVALID_SET_FILE_POINTER)
    {
      return false;
    }
  
  return true;
}


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

  hfile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  if (hfile == INVALID_HANDLE_VALUE)
    {
      return false;
    }
  if (GetFileInformationByHandle(hfile, &info) == FALSE)
    return false;

  switch (q)
    {
    case DOES_EXIST:
      ret = true;
      break;

    case IS_DIRECTORY:
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
  DWORD attr;
  bool ret;

  attr = GetFileAttributes(filename);
  if (attr == INVALID_FILE_ATTRIBUTES)
    return false;

  ret = false;

  if (q == IS_DIRECTORY)
    {
      if (attr == FILE_ATTRIBUTE_DIRECTORY)
	ret = directory_query_about(filename, q, aux);
    }
  else
    ret = normalfile_query_about(filename, q, aux);

  return ret;
}

bool	win32::file::exists(const char* filename)
{
  return file_query_about(filename, DOES_EXIST, 0);
}

bool	win32::file::is_directory(const char* filename)
{
  return file_query_about(filename, IS_DIRECTORY, 0);
}

// Currently, those functions always return true
bool	win32::file::is_readable(const char* filename)
{
  return file_query_about(filename, IS_READABLE, 0);
}

bool	win32::file::is_writtable(const char* filename)
{
  return file_query_about(filename, IS_WRITTABLE, 0);
}

bool	win32::file::is_executable(const char* filename)
{
  return file_query_about(filename, IS_EXECUTABLE, 0);
}

bool	win32::file::size(const char* filename, unsigned long* sz)
{
  return file_query_about(filename, GET_SIZE, sz);
}
