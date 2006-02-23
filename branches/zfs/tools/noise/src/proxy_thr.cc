//
// proxy_thr.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 10:58:42 2006 texane
// Last update Thu Feb 23 15:09:28 2006 texane
//


#include <iostream>
#include <noise.hh>


using namespace std;
using namespace sysapi;


#define BUFSZ 512


void* proxy::thr_receiver_entry(request_t* req)
{
  // receive from the server, send back to client

  buffer buf;
  error::handle_t sys_err;
  unsigned int nr_recv;
  unsigned int nr_sent;
  bool done;

  cout << "[ ] new recveiver thread" << endl;
  done = false;
  buf.resize(BUFSZ);
  while (done == false)
    {
      sys_err = insock::recv(req->srv_handle, buf.bufptr(), buf.size(), nr_recv);
      if (sys_err != error::SUCCESS)
	{
	  done = false;
	}
      else
	{
	  sys_err = insock::send(req->cli_handle, buf.bufptr(), nr_recv, nr_sent);
	  if (sys_err != error::SUCCESS)
	    {
	      done = false;
	    }
	}
    }

  cout << "[x] new recveiver thread" << endl;
  return 0;
}


void* proxy::thr_fuzzer_entry(request_t* req)
{
  // receive from the client, fuzz and send to server

  buffer buf;
  error::handle_t sys_err;
  unsigned int nr_recv;
  unsigned int nr_sent;
  bool done;

  cout << "[ ] new fuzzer thread" << endl;
  done = false;
  buf.resize(BUFSZ);
  while (done == false)
    {
      sys_err = insock::recv(req->cli_handle, buf.bufptr(), buf.size(), nr_recv);
      if (sys_err != error::SUCCESS)
	{
	  done = false;
	}
      else
	{
	  sys_err = insock::send(req->srv_handle, buf.bufptr(), nr_recv, nr_sent);
	  if (sys_err != error::SUCCESS)
	    {
	      done = false;
	    }
	}
    }

  cout << "[x] new fuzzer thread" << endl;
  return 0;
}
