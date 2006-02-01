//
// ztclient.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Dec 06 12:07:39 2005 texane
// Last update Wed Feb 01 19:05:05 2006 texane
//


#ifndef ZT_CLIENT_HH
# define ZT_CLIENT_HH


#include <sysapi/sysapi.hh>
#include <map>
#include <string>
#include <windows.h>


namespace zt
{
  class spool;
  class client
  {
    friend class spool;

  public:
    client(spool* spoolAddr,
	   const std::string& srvName,
	   unsigned short srvPort,
	   unsigned int nId,
	   const std::string& fileName,
	   unsigned char* ptrMapping,
	   DWORD fileLength,
	   bool renew = false);
    ~client();

    // Request the server
    bool requestServer();

    // Reporting methods
    void report() const;
    std::string toString() const;
    std::string idToString() const;


  private:

    // Spool related
    spool* m_spoolAddr;

    // Has the thread failed
    typedef enum
      {
	NOTSTARTED = 0,
	NOTCONNECTED,
	NOTSENT,
	NOTRECV,
	ABORTED,
	COMPLETED
      } rqstStatus;
    rqstStatus m_rqstStatus;

    // Configuration related
    std::map<std::string, std::string> m_conf;
    bool m_done;
    bool m_renew;
    std::string m_fileName;
    unsigned int m_nId;

    // Network related
    std::string m_srvName;
    unsigned short m_srvPort;

    // Resource related
    sysapi::socket_in::handle_t m_sockHandle;
    unsigned char* m_ptrMapping;
    DWORD m_fileLength;
    sysapi::socket_in::size_t m_nread;

    // Threading related
    static sysapi::thread::retcode_t thrEntry(sysapi::thread::param_t);
    sysapi::thread::handle_t m_thrHandle;
    HANDLE m_condvarWaitToStart;
    HANDLE m_condvarDone;
    
    // Timing information
    SYSTEMTIME m_tmStart;
    SYSTEMTIME m_tmEnd;

    // internal management methods
    void reset();

    // internal network related methods
    bool connectServer();
    bool closeServer();
    bool sendToServer();
    bool recvFromServer();
  };
}


#endif // ! ZT_CLIENT_HH
