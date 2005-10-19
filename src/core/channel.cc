//
// channel.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 15:07:37 2005 texane
// Last update Wed Oct 19 23:13:37 2005 
//


#include <string>
#include <cstring>
#include <iostream>
#include <server.hh>
#include <dataman.hh>


using std::cout;
using std::string;
using server::channel;


// One worker at a time server model;
// That's, one connection is open, then
// close and so on.


typedef struct
{
  sysapi::socket_in::handle_t hdl_con_;
  server::channel* chan_;
} session_t;


// Worker, that's a session
sysapi::thread::retcode_t worker_entry(sysapi::thread::param_t param)
{
  bool ret;
  char* ptr_line;
  session_t* sess = reinterpret_cast<session_t*>(param);

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


// Dispatcher
sysapi::thread::retcode_t server::channel::dispatcher_entry(sysapi::thread::param_t param)
{
  channel* chan = reinterpret_cast<channel*>(param);
  sysapi::socket_in::handle_t hdl_con;
  sysapi::thread::handle_t hdl_worker;
  // local variable, incorrect, but works for a one at a time server model
  session_t sess;

  // Accept on the socket
  while (1)
    {
      if (sysapi::socket_in::accept(&hdl_con, chan->hdl_accept_) == false)
	{
	  sysapi::error::stringify("Dispatcher cannot accept:");
	  sysapi::socket_in::terminate_connection(hdl_con);
	  return 0;
	}
      sysapi::thread::say("Dispatcher has accepted new connection");
      
      sess.chan_ = chan;
      sess.hdl_con_ = hdl_con;
      if (sysapi::thread::create_and_exec(&hdl_worker, worker_entry, reinterpret_cast<sysapi::thread::param_t>(&sess)) == false)
	{
	  sysapi::error::stringify("Cannot create worker thread");
	  sysapi::socket_in::terminate_connection(hdl_con);
	}
      else
	{
	  sysapi::thread::wait_single(hdl_worker);
	  sysapi::thread::release(hdl_worker);
	  sysapi::socket_in::terminate_connection(hdl_con);
	}
    }

  // Terminate
  sysapi::socket_in::terminate_connection(chan->hdl_accept_);
  return 0;
}


// Channel construction
server::channel::channel(unsigned short local_port, const char* local_addr)
{
}

server::channel::channel(const char* local_port, const char* local_addr)
{
  unsigned short port = (unsigned short)::atoi(local_port);

  // Create a listening socket for this channel
  if (sysapi::socket_in::create_listening(&hdl_accept_, port, local_addr) == false)
    {
      sysapi::error::stringify("Dispatcher cannot listen");
      throw (int)0;
    }
  sysapi::thread::say("Listening socket created");

  // Create the dispatcher thread
  if (sysapi::thread::create_and_exec(&hdl_dispatcher_, dispatcher_entry, reinterpret_cast<sysapi::thread::param_t>(this)) == false)
    {
      sysapi::error::stringify("Cannot create channel dispatcher thread");
      throw (int)0;
    }
}


// Channel destruction
server::channel::~channel()
{
  // release mutexes
  // close connections
}
