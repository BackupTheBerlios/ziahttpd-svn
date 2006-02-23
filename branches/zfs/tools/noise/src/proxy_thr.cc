//
// proxy_thr.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 10:58:42 2006 texane
// Last update Thu Feb 23 22:45:47 2006 texane
//


#include <iostream>
#include <noise.hh>
#include <proxy.hh>


using namespace std;
using namespace sysapi;


#define BUFSZ 512


void* proxy::thr_handle_remote(request_t* req)
{
  // srv -> us -> client

  buffer buf;
  error::handle_t sys_err;
  unsigned int nr_recv;
  unsigned int nr_sent;
  bool done;

  cout << "[ ] new remote thread" << endl;
  done = false;
  buf.resize(BUFSZ);
  while (done == false)
    {
      sys_err = insock::recv(req->remote_handle, buf.bufptr(), (unsigned int)buf.size(), nr_recv);
      if (sys_err != error::SUCCESS)
	{
	  done = true;
	}
      else
	{
	  sys_err = insock::send(req->local_handle, buf.bufptr(), nr_recv, nr_sent);
	  if (sys_err != error::SUCCESS)
	    {
	      done = true;
	    }
	}
    }

  cout << "remote thread done" << endl;
  req->thr_remote_done = true;
  return 0;
}


void* proxy::thr_handle_local(request_t* req)
{
  // receive from the client, fuzz and send to server

  buffer src;
  buffer dst;
  error::handle_t sys_err;
  unsigned int nr_recv;
  unsigned int nr_sent;
  bool done;

  cout << "[ ] new local thread" << endl;
  done = false;
  src.resize(BUFSZ);
  while (done == false)
    {
      sys_err = insock::recv(req->local_handle, src.bufptr(), (unsigned int)src.size(), nr_recv);
      if (sys_err != error::SUCCESS)
	{
	  done = true;
	}
      else
	{
	  src.resize(nr_recv);
// 	  req->px->m_noiser->fuzz(dst, src);
	  dst = src;
	  cout << dst.tostring() << endl;
	  sys_err = insock::send(req->remote_handle, dst.bufptr(), (unsigned int)dst.size(), nr_sent);
	  if (sys_err != error::SUCCESS)
	    {
	      done = true;
	    }
	}
    }

  req->thr_local_done = true;
  cout << "local thread now done\n" << endl;
  return 0;
}
