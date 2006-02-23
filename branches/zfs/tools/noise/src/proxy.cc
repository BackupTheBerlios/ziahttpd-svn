//
// proxy.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:56:27 2006 texane
// Last update Thu Feb 23 17:18:15 2006 texane
//


#include <list>
#include <iostream>
#include <proxy.hh>
#include <noise.hh>


using namespace std;
using namespace sysapi;


// internal

static void request_reset(request_t* req)
{
  req->thr_remote_done = false;
  req->thr_local_done = false;
}

static void request_release(request_t* req)
{
  if (req)
    {
      insock::close(req->local_handle);
      insock::close(req->remote_handle);
      delete req;
    }
}

static request_t* request_create(proxy* px)
{
  request_t* rq;

  rq = new request_t;
  request_reset(rq);
  rq->px = px;

  return rq;
}


bool proxy::handle_request(request_t*& req)
{
  // create the two threads here
  pthread_create(&req->thr_remote_id, 0, (void*(*)(void*))thr_handle_remote, req);
  pthread_create(&req->thr_local_id, 0, (void*(*)(void*))thr_handle_local, req);
  
  return true;
}

bool proxy::handle_connection(request_t*& req)
{
  error::handle_t sys_err;
  int nret;

  // accept connection
  req = request_create(this);
  cout << "[ ] accepting new connection" << endl;
  sys_err = insock::accept(req->local_handle, req->local_inaddr, req->px->m_local_handle);
  if (sys_err != error::SUCCESS)
    {
      delete req;
      req = 0;
      cout << "error accepting" << endl;
      return false;
    }

  // establish a connection with the server
  req->remote_handle = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (req->remote_handle == SOCKET_ERROR)
    {
      printf("cannot create socket fwd\n");
      fflush(stdout);
      return false;
    }
  nret = ::connect(req->remote_handle, (const struct sockaddr*)&req->px->m_remote_inaddr, sizeof(struct sockaddr_in));
  if (nret == SOCKET_ERROR)
    {
      printf("cannot connect to the server\n");
      return false;
    }
  cout << "[x] accepting new connection" << endl;  

  return true;
}

bool proxy::bind()
{
  error::handle_t sys_err;

  // bind the proxy
  if (m_bound == false)
    {
      cout << "[ ] binding the proxy" << endl;
      sys_err = insock::create_listening(m_local_handle, m_local_inaddr, 10);
      if (sys_err != error::SUCCESS)
	{
	  cout << "error binding" << endl;
	  m_done = true;
	  return false;
	}
      m_bound = true;
      cout << "[x] binding the proxy" << endl;
    }
  return false;
}


void proxy::reset()
{
  m_noiser = 0;

  m_done = false;
  m_log = DFLT_LOG;
  m_logfile = DFLT_LOGFILE;

  m_bound = false;
  m_local_port = DFLT_LOCAL_PORT;
  m_local_ip = DFLT_LOCAL_IP;

  m_remote_port = DFLT_REMOTE_PORT;
  m_remote_ip = DFLT_REMOTE_IP;
}


// exported

proxy::proxy(int ac, char** av)
{
  // list of options
  insock::init_subsystem();
  reset();
  insock::n_to_inaddr(m_local_inaddr, m_local_ip, m_local_port);
  insock::p_to_inaddr(m_remote_inaddr, m_remote_ip, m_remote_port);
  // m_noiser = new rnd;
  m_noiser = new http;
}


proxy::~proxy()
{
  if (m_noiser)
    {
      delete m_noiser;
      m_noiser = 0;
    }
  insock::release_subsystem();
}


#ifndef SOCKET_ERROR
# define SOCKET_ERROR -1
#endif // SOCKET_ERROR


bool proxy::forward()
{
  bool ret;
  request_t* req;
  fd_set rd_set;
  int nr_ret;
  struct timeval tm_timeout;

  ret = true;
  bind();
  while (m_done == false)
    {
      FD_ZERO(&rd_set);
      FD_SET(m_local_handle, &rd_set);
      tm_timeout.tv_sec = 0;
      tm_timeout.tv_usec = 500000;
      nr_ret = select((int)m_local_handle + 1, &rd_set, 0, 0, &tm_timeout);

      if (nr_ret == SOCKET_ERROR)
	{
	  // error
	  cout << "error selecting" << endl;
	}
      else if (nr_ret == 1)
	{
	  // Handle incoming connection
	  printf("on connection avaialbel\n"); fflush(stdout);
	  ret = handle_connection(req);
	  if (ret == true)
	    {
	      ret = handle_request(req);
	      if (ret == false)
		m_done = false;
	    }
	  else
	    m_done = false;
	}
      else
	{
	  // timeout, reap requests
	  list<request_t*>::iterator curr_req = m_req.begin();
	  list<request_t*>::iterator last_req = m_req.end();

	  while (curr_req != last_req)
	    {
	      if ((*curr_req)->thr_is_done)
		{
		  cout << "[ ] handling done request" << endl;
		}
	      ++curr_req;
	    }
	}
    }
  if (req)
    delete req;
  return ret;
}


void proxy::display_history() const
{
}
