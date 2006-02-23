//
// proxy.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:58:01 2006 texane
// Last update Thu Feb 23 20:40:29 2006 texane
//


#ifndef PROXY_HH
# define PROXY_HH


#include <list>
#include <string>
#include <buffer.hh>
#include <noise.hh>
#include <sched.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sysapi.hh>


// default options
// local designs the endpoint recving from the client
#define DFLT_LOCAL_PORT	40001
#define DFLT_LOCAL_IP INADDR_ANY
// remote designs endpoint recving from the server
#define DFLT_REMOTE_PORT 40000
#define DFLT_REMOTE_IP "localhost"
// general options
#define DFLT_LOG true
#define DFLT_LOGFILE "noise.out"


class proxy;


typedef struct
{
  proxy* px;

  // us -> client
  sysapi::insock::handle_t local_handle;
  struct sockaddr_in local_inaddr;
  pthread_t thr_local_id;
  bool thr_local_done;

  // us -> server
  sysapi::insock::handle_t remote_handle;
  pthread_t thr_remote_id;
  bool thr_remote_done;

} request_t;


class proxy
{
  friend request_t;

public:
  proxy(int, char**);
  ~proxy();
  bool forward();
  void display_history() const;

private:
  bool m_done;
  bool m_log;
  bool m_bound;

  // noiser
  noise* m_noiser;

  // client -> us
  unsigned short m_local_port;
  unsigned long m_local_ip;
  struct sockaddr_in m_local_inaddr;
  sysapi::insock::handle_t m_local_handle;

  // us -> server
  unsigned short m_remote_port;
  std::string m_remote_ip;
  struct sockaddr_in m_remote_inaddr;
  sysapi::insock::handle_t m_remote_handle;

  // request queue
  std::list<request_t*> m_req;
  
  // logging
  std::string m_logfile;
  std::list<std::string> m_hist;


  // internal management
  void reset();
  bool bind();
  bool handle_request(request_t*&);
  bool handle_connection(request_t*&);
  static void* thr_handle_local(request_t*);
  static void* thr_handle_remote(request_t*);
};


#endif // ! PROXY_HH
