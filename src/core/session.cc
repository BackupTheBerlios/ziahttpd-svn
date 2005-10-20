//
// session.cc for  in /home/texane/texane/wip/ept3/zia_work/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 19 23:29:57 2005 
// Last update Thu Oct 20 19:07:35 2005 
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
  is_persistent_ = true;
}


// Destruction
server::session::~session()
{
}


// Get chunked body
bool server::session::get_body(socket_in::size_t sz_body)
{
  bool ret;
  unsigned char* buf;
  socket_in::size_t nr_recv;

  buf = new unsigned char[sz_body];
  ret = socket_in::recv(hdl_con_, buf, sz_body, &nr_recv);
  if (ret == false)
    {
      buf = NULL;
      delete buf;
    }

  return ret;
}


// Get non chunked body
bool server::session::get_body()
{
  return false;
}


// Session worker thread
sysapi::thread::retcode_t server::session::worker_entry_(sysapi::thread::param_t param)
{
  bool			ret;
  char*			ptr_line;
  socket_in::error_t	err;
  server::session*	sess = reinterpret_cast<server::session*>(param);
  http::message		msg(*sess);

  thread::say("Servicing the new client");

  do
    {
      // request line
      thread::say("Servicing the new request");
      while ((ret = dataman::get_nextline(sess->hdl_con_, &ptr_line, &err)) == true && !::strlen((const char*)ptr_line))
	delete[] ptr_line;
      if (ret == true)
	{
	  msg.statusline((const char*)ptr_line);
	  delete[] ptr_line;
	}
      else
	{
	  if (err == sysapi::socket_in::CONN_DISCONNECTED)
	    return 0;
	}

      // header
      while ((ret = dataman::get_nextline(sess->hdl_con_, &ptr_line, &err)) && strlen((const char*)ptr_line))
	{
	  msg.header((const char*)ptr_line);
	  delete[] ptr_line;
	}
      if (ret == true)
	delete[] ptr_line;
      else
	{
	  if (err == socket_in::CONN_DISCONNECTED)
	    return 0;
	}

      // body
      sess->get_body();
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
