//
// error.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 00:20:24 2006 texane
// Last update Tue Jan 24 23:45:37 2006 texane
//


#include <string>
#include <sstream>
#include <iostream>
#include <core/ziafs_status.hh>


using std::cerr;
using std::endl;
using std::string;
using std::ostringstream;


void status::debug_err_fn(error e, const char* filename, int nline)
{
  cerr << "(@" << filename << "#" << nline << "): " << tostring(e) << endl;
}


void status::log_err_fn(error e, const char* filename, int nline)
{
  cerr << "(@" << filename << "#" << nline << "): " << tostring(e) << endl;
}


string status::dump(error e, const char* filename, int nline)
{
  ostringstream strm;

  strm << "(@" << filename << "#" << nline << "): " << tostring(e);
  return strm.str();
}


#define on_case( e , s )			\
 case e:					\
   str = s;					\
   break;

string status::tostring(error e)
{
  const char* str;

  switch (e)
    {
      on_case( SUCCESS, "operation success" );
      on_case( NOTIMPL, "feature not implemented" );
      on_case( PARTIALIMPL, "feature partially implemented" );
    default:
      str = "unknown error";
    }

  return string(str);
}
