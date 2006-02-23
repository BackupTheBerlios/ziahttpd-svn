//
// proxy.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:56:27 2006 texane
// Last update Thu Feb 23 15:15:50 2006 texane
//


#include <list>
#include <iostream>
#include <noise.hh>


using namespace std;
using namespace sysapi;


// internal

static void request_reset(request_t& req)
{
  req.thr_is_done = false;
}

static void request_release(request_t& req)
{
  if (req)
    {
      pthread_mutex_release(&req->cli_buf_lock);
      pthread_mutex_release(&req->srv_buf_lock);
      delete req;
    }
}

static request_t* request_create()
{
  request_t* rq;
  int ret;

  rq = new request_t;
  request_reset(*rq);
  pthread_mutex_init(&rq->cli_buf_lock, 0);
  pthread_mutex_init(&rq->srv_buf_lock, 0);

  return rq;
}


bool proxy::handle_request(request_t*& req)
{
  pthread_create();
  return true;
}

bool proxy::handle_connection(request_t*& req)
{
  error::handle_t sys_err;

  // accept connection
  req = request_create();
  cout << "[ ] accepting new connection" << endl;
  sys_err = insock::accept(req->cli_handle, req->cli_inaddr, m_srv_handle);
  if (sys_err != error::SUCCESS)
    {
      delete req;
      req = 0;
      cout << "error accepting" << endl;
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
      sys_err = insock::create_listening(m_srv_handle, m_srv_inaddr, 10);
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
  m_done = false;
  m_local_port = DFLT_LOCAL_PORT;
  m_local_ip = DFLT_LOCAL_IP;
  m_log = DFLT_LOG;
  m_logfile = DFLT_LOGFILE;
  m_bound = false;
}


// exported

proxy::proxy(int ac, char** av)
{
  // list of options
  insock::init_subsystem();
  reset();
  insock::n_to_inaddr(m_srv_inaddr, m_local_ip, m_local_port);
}


proxy::~proxy()
{
  insock::release_subsystem();
}


#ifndef SOCKET_ERROR
# define SOCKET_ERROR -1
#endif // SOCKET_ERROR


bool proxy::fuzz()
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
      FD_SET(m_srv_handle, &rd_set);
      tm_timeout.tv_sec = 0;
      tm_timeout.tv_usec = 500000;
      cout << "[ ] select" << endl;
      nr_ret = select((int)m_srv_handle + 1, &rd_set, 0, 0, &tm_timeout);
      cout << "[x] select" << endl;

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
