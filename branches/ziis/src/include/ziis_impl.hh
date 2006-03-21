//
// ziis_impl.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 18:49:56 2006 texane
// Last update Tue Mar 21 22:42:59 2006 texane
//


#ifndef ZIIS_IMPL_HH
# define ZIIS_IMPL_HH


#include <ziafs_thr.hh>
#include <ziafs_http.hh>
#include <sys/sysapi.hh>
#include <ziis.hh>


class ZfsInput : public IInput
{
public:
  ZfsInput(net::http&);
	ZfsInput(thr::pool::session_t&);
  ~ZfsInput();

  const char* GetInput(const char*);
  const char* GetInputMethod();
  const char* GetInputLocation();
  const char* GetInputHttpVersion();
  const char* GetInputQueryString();
  int					GetClientIp();
  int ReadPostEntity(char*, int){ return -1;}
  bool GetNextHeader(char**, char**);

private:
  net::http*						m_proto;
	thr::pool::session_t* m_session;
	int										m_ip_client;
};


class ZfsOutput : public IOutput
{
public:
	ZfsOutput(net::http&);
	ZfsOutput(thr::pool::session_t&);
	~ZfsOutput() {}

  void SetOutput(const char*, const char*);
  const char* GetOutput(const char*);
  bool SendHeader();
  int SendError(int) { return -1;}
  int SendBuffer(const char*, int) { return -1; }
  void SetStatusCode(int);

private:
  net::http* m_proto;
	thr::pool::session_t* m_session;
};


#endif // ! ZIIS_IMPL_HH
