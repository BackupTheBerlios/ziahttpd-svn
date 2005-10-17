//
// test_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Mon Oct 10 18:34:44 2005 texane
// Last update Mon Oct 10 19:05:56 2005 texane
//

#include <iostream>
#include <cstring>
#include <sysapi.hh>

static void putstr_to_file(sysapi::file::handle_t hdl, const unsigned char* str)
{
  sysapi::file::size_t tot = 0;
  sysapi::file::size_t cnt;
  bool ret;

  while (*str && ((ret = sysapi::file::write(hdl, str, 1, &cnt)) == true))
    {
      tot += cnt;
      ++str;
    }

  if (ret == false)
    {
      sysapi::error::stringify("ERROR:");
    }
  else
    {
      std::cout << "written " << tot << " bytes to file\n";
    }
}

void	test_file(const char* filename, const unsigned char* buf)
{
  sysapi::file::handle_t hdl;
  bool ret;

  std::cout << "opening file\n";
  ret = sysapi::file::open(&hdl, filename, sysapi::file::WRONLY);
  
  if (ret == false)
    {
      sysapi::error::stringify("Cannot open file:");
    }
  else
    {
      std::cout << "File successfully created\n";
      sysapi::file::seek(hdl, 0, sysapi::file::END);
      putstr_to_file(hdl, (const unsigned char*)"Im just a simple string...");
      std::cout << "closing file\n";
      if (sysapi::file::close(hdl) == false)
	{
	  sysapi::error::stringify("ERROR CLOSING FILE:");
	}
    }  
}
