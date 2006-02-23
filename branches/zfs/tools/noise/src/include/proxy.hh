//
// proxy.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Feb 23 09:58:01 2006 texane
// Last update Thu Feb 23 15:12:06 2006 texane
//


#ifndef PROXY_HH
# define PROXY_HH


#include <list>
#include <string>
#include <sched.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/sysapi.hh>


#define DFLT_LOCAL_PORT 40000
#define DFLT_LOCAL_IP INADDR_ANY
#define DFLT_LOG true
#define DFLT_LOGFILE "noise.out"


typedef struct
{
  // network related
  sysapi::insock::handle_t srv_handle;
  struct sockaddr_in srv_inaddr;
  sysapi::insock::handle_t cli_handle;
  struct sockaddr_in cli_inaddr;

  // received from the client
  pthread_mutex_t cli_buf_lock;
  buffer cli_buf;
  // received from the server
  pthread_mutex_t srv_buf_lock;
  buffer srv_buf;

  // thread related
  pthread_t thr_recv_id;
  pthread_t thr_fuzzer_id;


  pthread_cond_t thr_cond_done;
  pthread_mutex_t thr_mtx_done;
  bool thr_is_done;
} request_t;


class proxy
{
public:
  proxy(int, char**);
  ~proxy();
  bool fuzz();
  void display_history() const;

private:
  bool m_done;
  bool m_log;
  bool m_bound;
  unsigned short m_local_port;
  unsigned long m_local_ip;
  struct sockaddr_in m_srv_inaddr;
  sysapi::insock::handle_t m_srv_handle;
  std::string m_logfile;
  std::list<std::string> m_hist;
  std::list<request_t*> m_req;

  void reset();
  bool bind();
  bool handle_request(request_t*&);
  bool handle_connection(request_t*&);
  static void* thr_receiver_entry(request_t*);
  static void* thr_fuzzer_entry(request_t*);
};


#endif // ! PROXY_HH
