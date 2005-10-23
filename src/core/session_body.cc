//
// session_body.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Oct 22 17:37:54 2005 texane
// Last update Sun Oct 23 23:49:35 2005 texane
//


// There should be only one execution point
// to fill the body of a repsonse in.
// That's, if the file is a cgi-bin, call
// the function cgi-bin(), is this is an
// html file, call the function readfile()...
// In this fashion, there is only one point to
// fill the body in


#include <server.hh>
#include <map>
#include <iostream>


using namespace sysapi;
using namespace http;


bool server::session::body_fetch_from_file()
{
  sysapi::file::handle_t hdl_file;
  unsigned long sz_file;

  sysapi::thread::say("There is file a to fetch from");
  sysapi::thread::say(http_info_.filename_);

  if (sysapi::file::size(http_info_.filename_, &sz_file) == false)
    {
      return false;
    }

  if (sysapi::file::open(&hdl_file, http_info_.filename_, sysapi::file::RDONLY) == false)
    {
      sysapi::error::stringify("Cannot open file");
      return false;
    }

  http_info_.buf_body_ = new unsigned char[sz_file];
  sysapi::file::read(hdl_file, http_info_.buf_body_, sz_file);

  http_info_.sz_body_ = sz_file;
  
  sysapi::file::close(hdl_file);
  delete[] http_info_.filename_;
  return true;
}


// !Fixme: only supports get method
static void add_to_buf(unsigned char** dst, const unsigned char* src, sysapi::file::size_t sz_dst, sysapi::file::size_t sz_src)
{
  unsigned char* res;
  sysapi::file::size_t i;
  sysapi::file::size_t j;

  res = new unsigned char[sz_dst + sz_src];
  for (i = 0; i < sz_dst; ++i)
    res[i] = (*dst)[i];
  for (j = 0; j < sz_src; ++j, ++i)
    res[i] = src[j];

  delete[] *dst;
  *dst = res;
}


bool server::session::body_fetch_from_cgibin()
{
  sysapi::process::handle_t hprocess;
  sysapi::file::handle_t hread;
  sysapi::file::handle_t hwrite;
  sysapi::file::size_t nwrite;
  char** env;
  char* av[2];
  int ac;
  bool ret;

  sysapi::thread::say("There is a cgi script to execute");
  sysapi::thread::say(http_info_.filename_);

  ac = 1;
  av[0] = http_info_.filename_;
  av[1] = 0;
  env = 0;

  if (http_info_.is_method_get_ == true)
    {
      if (sysapi::process::create_outredir_and_loadexec(&hprocess, &hread, ac, (const char**)av, (const char**)env) == true)
	{
	  // Fetch the buffer from hread
	  http::dataman::buffer body(hread);
	  http_info_.buf_body_ = body.dup();
	  std::cout << "DISPLAYING body" << std::endl;
	  body.display();
	  http_info_.sz_body_ = (unsigned long)body.size();

	  // Wait for the child to end
	  sysapi::process::wait_single(hprocess);
	  sysapi::process::release(hprocess);
	  sysapi::file::close(hread);
	}
      else
	{
	  sysapi::error::stringify("Creating process");
	  ret = false;
	}
    }
  else if (http_info_.is_method_post_ == true)
    {
      if (sysapi::process::create_inoutredir_and_loadexec(&hprocess, &hread, &hwrite, ac, (const char**)av, (const char**)env) == true)
	{
	  if (http_info_.buf_body_)
	    {
	      sysapi::file::write(hwrite, (const unsigned char*)http_info_.buf_body_, http_info_.sz_body_, &nwrite);
	      sysapi::file::close_wr(hwrite);
	      delete[] http_info_.buf_body_;
	      http_info_.buf_body_ = 0;
	    }

	  // Fetch the body from hread
	  http::dataman::buffer body(hread);
	  http_info_.sz_body_ = (unsigned long)body.size();
	  http_info_.buf_body_ = body.dup();

	  // Close the read part of the pipe and wait child
	  sysapi::file::close_rd(hread);
	  sysapi::process::wait_single(hprocess);
	  sysapi::process::release(hprocess);
	}
      else
	{
	  sysapi::error::stringify("Creating process");
	  ret = false;
	}
    }
  else
    {
      std::cout << "Cannot create process!" << std::endl;
      ret = false;
    }

  // delete the cgi name
  delete[] http_info_.filename_;

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
