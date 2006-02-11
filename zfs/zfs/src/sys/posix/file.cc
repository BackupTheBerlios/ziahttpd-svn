//
// file.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:01:40 2006 
// Last update Sat Feb 11 18:10:20 2006 
//

#include <string>
#include <sys/sysapi.hh>


using std::string;


sysapi::error::handle_t sysapi::file::open(handle_t& hfile, const std::string& path)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::close(handle_t& hfile)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::read(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nread)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::write(handle_t& hfile, unsigned char* buf, unsigned int nbytes, unsigned int& nwritten)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::file::size(handle_t& hfile, unsigned long long& sz)
{
  return error::SUCCESS;
}
