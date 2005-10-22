//
// session_body.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Oct 22 17:37:54 2005 texane
// Last update Sat Oct 22 17:57:01 2005 texane
//


// There should be only one execution point
// to fill the body of a repsonse in.
// That's, if the file is a cgi-bin, call
// the function cgi-bin(), is this is an
// html file, call the function readfile()...
// In this fashion, there is only one point to
// fill the body in


#include <server.hh>
#include <iostream>


using namespace sysapi;
using namespace http;


bool server::session::body_fetch_from_file()
{
  sysapi::thread::say("There is a filename");
  sysapi::thread::say(http_info_.filename_);
  delete[] http_info_.filename_;
  return true;
}


// !Fixme: only supports get method
bool server::session::body_fetch_from_cgibin()
{
  sysapi::thread::say("There is a cgi script to execute");
  sysapi::thread::say(http_info_.buf_cgi_);
  delete[] http_info_.buf_cgi_;
  return true;
}


bool server::session::body_fetch()
{
  bool ret;

  ret = true;
  if (http_info_.is_file_ == true)
    {
      ret = body_fetch_from_file();
    }
  else if (http_info_.is_cgi_ == true)
    {
      ret = body_fetch_from_cgibin();
    }
  else
    {
      // unknown file type
      ret = false;
    }

  return ret;
}
