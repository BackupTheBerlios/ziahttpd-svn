//
// channel.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Oct 12 15:07:37 2005 texane
// Last update Thu Oct 20 12:19:23 2005 
//


#include <string>
#include <cstring>
#include <iostream>
#include <server.hh>


using std::cout;
using std::string;
using server::channel;


// One worker at a time server model;
// That's, one connection is open, then
// close and so on.


// Dispatcher
sysapi::thread::retcode_t server::channel::dispatcher_entry(sysapi::thread::param_t param)
{
  channel* chan = reinterpret_cast<channel*>(param);
  sysapi::socket_in::handle_t hdl_con;
  session* sess;

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
      
      sess = new server::session(hdl_con, chan);
      if (sess->create_worker_thread() == true)
	{
	  sysapi::thread::wait_single(sess->get_worker_handle());
	  sysapi::thread::release(sess->get_worker_handle());
	  sysapi::socket_in::terminate_connection(sess->get_connection_handle());
	}
      delete sess;
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
