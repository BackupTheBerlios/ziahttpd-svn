//
// insock.cc for  in /home/texane/texane/tmp/zfs/ziahttpd/zfs/zfs
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sat Feb 11 17:01:50 2006 
// Last update Sat Feb 11 18:11:53 2006 
//


#include <string>
#include <sys/socket.h>
#include <sys/sysapi.hh>


using std::string;


sysapi::error::handle_t sysapi::insock::init_subsystem()
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::release_subsystem()
{
  return  error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::p_to_inaddr(struct sockaddr_in& inaddr, const string& addr, unsigned short port)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::n_to_inaddr(struct sockaddr_in& inaddr, unsigned long addr, unsigned short port)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::create_listening(handle_t& hsock, struct sockaddr_in& inaddr, unsigned int nbklog)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::accept(handle_t& hsock, struct sockaddr_in& inaddr, handle_t& hbound)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::recv(handle_t& hsock, unsigned char* buf, unsigned int nbytes, unsigned int& nrecv)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::send(handle_t& hsock, unsigned char* buf, unsigned int nbytes, unsigned int& nsent)
{
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::insock::close(handle_t& hsock)
{
  return error::SUCCESS;
}
