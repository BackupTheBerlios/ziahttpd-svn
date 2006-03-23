//
// ziis_impl.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 18:49:56 2006 texane
// Last update Wed Mar 22 21:47:13 2006 texane
//


#ifndef ZIIS_IMPL_HH
# define ZIIS_IMPL_HH


#include <list>
#include <ziafs_thr.hh>
#include <ziafs_http.hh>
#include <sys/sysapi.hh>
#include <ziis.hh>


class ZfsInput : public IInput
{
public:
  ZfsInput(thr::pool::session_t&);
	ZfsInput(thr::pool::session_t*);
  ~ZfsInput();

  const char* GetInput(const char*);
  const char* GetInputMethod();
  const char* GetInputLocation();
  const char* GetInputHttpVersion();
  const char* GetInputQueryString();
  int GetClientIp();
  int ReadPostEntity(char*, int);
  bool GetNextHeader(char**, char**);

private:
	bool genete_type_mimes(net::config& conf, std::string& extension, std::string& type);
	net::http* m_proto;
  thr::pool::session_t* m_session;
  int										m_ip_client;

};


class ZfsOutput : public IOutput
{
public:
  ZfsOutput(net::http&);
  ZfsOutput(thr::pool::session_t&);
	ZfsOutput(thr::pool::session_t*);
  ~ZfsOutput() {}

  void SetOutput(const char*, const char*);
  const char* GetOutput(const char*);
  bool SendHeader();
  int SendError(int);
  int SendBuffer(const char*, int);
  void SetStatusCode(int);

private:
  net::http* m_proto;
  thr::pool::session_t* m_session;

  int send_whole_buffer(const char*, int, bool = false);
};


#endif // ! ZIIS_IMPL_HH
