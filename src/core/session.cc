//
// session.cc for  in /home/texane/texane/wip/ept3/zia_work/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 19 23:29:57 2005 
// Last update Thu Oct 20 01:23:15 2005 
//


#include <server.hh>


// Construction
server::session::session(sysapi::socket_in::handle_t hdl, server::channel* chan)
{
  hdl_con_ = hdl;
  this_chan_ = chan;
}


// Destruction
server::session::~session()
{
}


// Session worker thread
sysapi::thread::retcode_t server::session::worker_entry_(sysapi::thread::param_t param)
{
  bool ret;
  char* ptr_line;
  server::session* sess = reinterpret_cast<server::session*>(param);

  sysapi::thread::say("Servicing the new client");
  
  // request line
  while ((ret = http::dataman::get_nextline(sess->hdl_con_, &ptr_line)) == true &&
	 !::strlen((const char*)ptr_line))
    {
      sysapi::thread::say("<CRLF>");
      delete[] ptr_line;
    }
  if (ret == true)
    {
      sysapi::thread::say("<REQUEST_LINE>");
      delete[] ptr_line;
    }

  // header
  while ((ret = http::dataman::get_nextline(sess->hdl_con_, &ptr_line)) && strlen((const char*)ptr_line))
    {
      sysapi::thread::say("<HEADER_LINE>");
      delete[] ptr_line;
    }
  if (ret == true)
    {
      sysapi::thread::say("<CRLF>");
      delete[] ptr_line;
    }

  // body
  sysapi::thread::say("<BODY>");  

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
  else
    {
      sysapi::thread::wait_single(hdl_worker_);
      sysapi::thread::release(hdl_worker_);
      sysapi::socket_in::terminate_connection(hdl_con_);
    }

  return true;
}
