//
// session.cc for  in /home/texane/texane/wip/ept3/zia_work/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 19 23:29:57 2005 
// Last update Fri Oct 21 18:26:40 2005 
//


#include <server.hh>
#include <iostream>


using namespace sysapi;
using namespace http;


// Construction
server::session::session(sysapi::socket_in::handle_t hdl, server::channel* chan)
{
  hdl_con_ = hdl;
  this_chan_ = chan;
  nr_timeout_ = server::session::MAX_IDLE_TIME;
  http_info_.is_persistent_ = true;
  http_info_.nr_rqst_ = 0;
}


// Destruction
server::session::~session()
{
}


// Session worker thread.
// It loops until the connection is no more persistent.
sysapi::thread::retcode_t server::session::worker_entry_(sysapi::thread::param_t param)
{
  socket_in::error_t	err;
  server::session*	sess = reinterpret_cast<server::session*>(param);
  unsigned char*	body;
  char*			ptr_line;

  // Main serverloop
  do
    {
      http::message	msg(sess);

      thread::say("Servicing the new request");
      sess->http_info_.is_body_ = false;
      sess->http_info_.is_chunked_ = false;

      // get http message
      sess->get_statusline(&ptr_line, &err);
      msg.statusline((const char*)ptr_line);
      delete[] ptr_line;
      while (sess->get_headerline(&ptr_line, &err) && ::strlen((const char*)ptr_line))
	{
	  sysapi::thread::say(ptr_line);
	  msg.header((const char*)ptr_line);
	  delete[] ptr_line;
	}

      // we are one the last crlf
      delete[] ptr_line;

      // get the body, if any
      sess->get_body(&body, &err);
    }
  while (sess->is_persistent() == true);

  return 0;
}



// Create a worker thread
bool server::session::create_worker_thread()
{
  if (sysapi::thread::create_and_exec(&hdl_worker_, worker_entry_, reinterpret_cast<sysapi::thread::param_t>(this)) == false)
    {
      sysapi::error::stringify("Cannot create worker thread");
      sysapi::socket_in::terminate_connection(hdl_con_);
      return false;
    }

  return true;
}
