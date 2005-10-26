//
// session.cc for  in /home/texane/texane/wip/ept3/zia_work/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 19 23:29:57 2005 
// Last update Wed Oct 26 16:02:08 2005 
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
  http::dataman::buffer buf;

  // Main serverloop
  do
    {
      bool ret;
      http::message	msg(sess);

      thread::say("Servicing the new request");

      // Reset http informations
      sess->reset_http_information();

      // Get http message
      if (!sess->get_statusline(buf, &err)) return 0;
      if (!msg.statusline(buf)) return 0;
      while ((ret = sess->get_headerline(buf, &err)) && buf.size())
	msg.header(buf);
      if (!ret) return 0;

      // Get the body, if any
      sess->get_body(sess->http_info_.request_body_, &err);
      
      // Set internal message informations
      msg.make_response();

      // Fetch the body
      sess->body_fetch();
      msg.bodysize(sess->http_info_.response_body_.size());

      // Stringify the response
      msg.stringify();

      // Send the repsonse
      if (sess->http_info_.buf_statusline_)
	{
	  sysapi::socket_in::send(sess->hdl_con_,
				  reinterpret_cast<const unsigned char*>(sess->http_info_.buf_statusline_),
				  static_cast<sysapi::socket_in::size_t>(strlen(sess->http_info_.buf_statusline_)));
	  delete[] sess->http_info_.buf_statusline_;
	}
      if (sess->http_info_.buf_headerlines_)
	{
	  sysapi::socket_in::send(sess->hdl_con_,
				  reinterpret_cast<const unsigned char*>(sess->http_info_.buf_headerlines_),
				  static_cast<sysapi::socket_in::size_t>(strlen(sess->http_info_.buf_headerlines_)));
	  delete[] sess->http_info_.buf_headerlines_;
	}

      // Send the body
      if (sess->http_info_.response_body_.size())
	sysapi::socket_in::send(sess->hdl_con_,
				(const unsigned char*)sess->http_info_.response_body_,
				(sysapi::socket_in::size_t)sess->http_info_.response_body_.size());
      
    }
  while (sess->is_persistent() == true);

  sysapi::thread::say("persistency is now false");
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


// Reset http related informations
void server::session::reset_http_information()
{
  // Are there ...
  http_info_.is_chunked_ = false;
  http_info_.is_cgi_ = false;
  http_info_.is_file_ = false;
  http_info_.is_method_get_ = false;
  http_info_.is_method_post_ = false;

  // Nullize the buffers
  http_info_.buf_statusline_ = 0;
  http_info_.buf_headerlines_ = 0;
  http_info_.filename_ = 0;
}
