//
// session_body.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Oct 22 17:37:54 2005 texane
// Last update Wed Oct 26 17:27:31 2005 
//


// There should be only one execution point
// to fill the body of a repsonse in.
// That's, if the file is a cgi-bin, call
// the function cgi-bin(), is this is an
// html file, call the function readfile()...
// In this fashion, there is only one point to
// fill the body in


#include <map>
#include <iostream>
#include <server.hh>
#include <buffer.hh>


using namespace sysapi;
using namespace http;
using http::dataman::buffer;
using std::cout;
using std::endl;


bool server::session::body_fetch_from_file()
{
  sysapi::file::handle_t	hfile;
  unsigned long			sz_file;
  unsigned char*		buf;

  sysapi::thread::say("There is file a to fetch from");
  sysapi::thread::say(http_info_.filename_);

  if (sysapi::file::size(http_info_.filename_, &sz_file) == false)
    return false;

  if (sysapi::file::open(&hfile, http_info_.filename_, sysapi::file::RDONLY) == false)
    {
      sysapi::error::stringify("Cannot open file");
      return false;
    }

  // ! FROM URI
  // http_info_.response_body_ = buffer(uri);
  buf = new unsigned char[sz_file];
  sysapi::file::read(hfile, buf, sz_file);
  http_info_.response_body_.buf(buf, sz_file);
  cout << "size: " << sz_file << endl;

  sysapi::file::close(hfile);
  delete[] http_info_.filename_;
  return true;
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
	  http_info_.response_body_ = body;

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
	  if (http_info_.request_body_.size())
	    {
	      sysapi::file::write(hwrite, (const unsigned char*)http_info_.request_body_, http_info_.request_body_.size(), &nwrite);
	      sysapi::file::close_wr(hwrite);
	    }

	  // Fetch the body from hread
	  http::dataman::buffer body(hread);
	  http_info_.response_body_ = body;

	  // Close the read part of the pipe and wait child
	  sysapi::file::close_rd(hread);
	  sysapi::process::wait_single(hprocess);
	  sysapi::process::release(hprocess);
	}
      else
	{
	  sysapi::thread::say("Unknown method type");
	  ret = false;
	}
    }
  else
    {
      sysapi::error::stringify("Creating process");
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
  if (http_info_.is_file_ == true)	ret = body_fetch_from_file();
  else if (http_info_.is_cgi_ == true)	ret = body_fetch_from_cgibin();
  else					ret = false;

  return ret;
}


bool	server::session::fetch_response_resource()
{
  bool	ret;

  ret = false;

  if (http_info_.response_res_.cgi())
    {
      ret = body_fetch_from_cgibin();
    }
  else if (http_info_.response_res_.raw())
    {
      ret = body_fetch_from_file();
    }
  else
    {}

  return ret;
}
