//
// mod_tmsock.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 08 14:29:08 2006 texane
// Last update Sat Apr 08 16:45:13 2006 texane
//


#ifndef MOD_TMSOCK_HH
# define MOD_TMSOCK_HH


#include <iostream>
#include <string>
#include <list>
#include <sys/sysapi.hh>
#include <ziis.hh>
#ifdef _WIN32
# include <sys/pthread.h>
# include <windows.h>
#else
# include <pthread.h>
#endif // _WIN32


class mod_tmsock : public IModule, public IConnection
{
public:
  // ctor/dtor
  mod_tmsock(const std::string& = "conf/ziafs.xml");
  ~mod_tmsock();

  // implement IModule
  bool ReadConfig(const char*);
  const char* GetModuleName();
  const char* GetModuleVersion();
  void OnLoad(IFS*);

  // implement IConnection
  short GetPort();
  const char* GetHost();
  void* Accept(SOCKET);
  int Recv(SOCKET, void*, char*, int);
  int Send(SOCKET, void*, const char*, int);
  void Close(SOCKET, void*);


private:
  std::string m_host;
  short m_port;

  // module are resetable
  void reset();
  void release();
  bool reload(const std::string&);

  // module accesses monitoring
  bool enter(SOCKET, void*);
  bool leave();
  pthread_mutex_t lk_iodescs;

  // timeout managment
  typedef struct
  {
    bool is_used;
    sysapi::time::handle_t tm_enter;
    SOCKET id_socket;
    unsigned long id_thread;
  } _iodesc_t;

  enum { nr_iodescs = 500 };
  _iodesc_t arr_iodescs[nr_iodescs];

  // timeouter thread related
  static void* thr_entry(void*);
  pthread_t id_thread;
};


#ifdef _WIN32
# define get_current_thrid() ((unsigned long)pthread_self().p)
#else
# define get_current_thrid() ((unsigned long)pthread_self())
#endif // _WIN32


#endif // ! MOD_TMSOCK_HH
