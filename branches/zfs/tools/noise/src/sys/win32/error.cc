//
// error.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 14:15:50 2006 texane
// Last update Sun Jan 22 15:54:45 2006 texane
//


#include <string>
#include <sys/sysapi.hh>


using std::string;


std::string sysapi::error::tostring(const handle_t& herr)
{
#define NR_MAX_BUF 256
  char buf[NR_MAX_BUF];
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, herr, 0, buf, sizeof(buf) - 1, 0);
  return std::string(buf);
}


bool sysapi::error::is_failure(const handle_t& herr)
{
  if (herr == ERROR_SUCCESS)
    return false;
  return true;
}


bool sysapi::error::is_success(const handle_t& herr)
{
  return !is_failure(herr);
}
