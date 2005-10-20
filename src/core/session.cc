//
// session.cc for  in /home/texane/texane/wip/ept3/zia_work/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 19 23:29:57 2005 
// Last update Thu Oct 20 20:16:59 2005 
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
  bool			ret;
  char*			ptr_line;
  server::session*	sess = reinterpret_cast<server::session*>(param);

  // Main serverloop
  do
    {
      http::message	msg(sess);

      thread::say("Servicing the new request");

      // skip the crlf
      ret = sess->skip_crlf(&ptr_line, &err);
      if (ret == false && err == sysapi::socket_in::CONN_DISCONNECTED)
	return 0;
      
      // we are on the status line
      msg.statusline((const char*)ptr_line);
      delete[] ptr_line;

      // get all headers
      while (sess->get_headerlines(&ptr_line, &err) && ::strlen((const char*)ptr_line))
	{
	  msg.header((const char*)ptr_line);
	  delete[] ptr_line;
	}

      // we are one the last crlf
      if (ret == true)
	delete[] ptr_line;
      else if (err == socket_in::CONN_DISCONNECTED)
	return 0;

      // get the body, if any
      // sess->get_body(0, 0, 0, 0);
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
