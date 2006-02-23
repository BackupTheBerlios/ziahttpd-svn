//
// proxy.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:56:27 2006 texane
// Last update Thu Feb 23 11:09:53 2006 texane
//


#include <iostream>
#include <noise.hh>


using namespace std;
using namespace sysapi;


// internal

static void request_reset(request_t& req)
{
  req.thr_is_done = false;
}

static request_t* request_create()
{
  request_t* rq;

  rq = new request_t;
  request_reset(*rq);
  return rq;
}

static void request_release(request_t* req)
{
  delete req;
}


bool proxy::handle_request(request_t*& req)
{
  return true;
}

bool proxy::handle_connection(request_t*& req)
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
	  return false;
	}
      m_bound = true;
      cout << "[x] binding the proxy" << endl;
    }

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
  reset();
}


proxy::~proxy()
{
  
}


bool proxy::fuzz()
{
  bool ret;
  request_t* req;

  ret = true;
  while (m_done == false)
    {
      req = 0;
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
  if (req)
    delete req;
  return ret;
}


void proxy::display_history() const
{
}
