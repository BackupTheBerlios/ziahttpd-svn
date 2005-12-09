//
// ztspool.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Dec 06 12:09:36 2005 texane
// Last update Wed Dec 07 21:48:22 2005 texane
//


#ifndef ZT_SPOOL_HH
# define ZT_SPOOL_HH


#include <map>
#include <list>
#include <string>
#include <cstring>
#include <ztclient.hh>
#include <windows.h>


namespace zt
{
  class spool
  {
  public:
    spool(int ac, char** av, char** env = 0);
    ~spool();

    bool startNext(client*& res, client* toStart = 0);
    bool waitNext(client*& res);
    bool handleEndofRequest(client* cli);
    HANDLE condvarWaitForClient() const;

    std::string idToString() const;

    // File mapping internal type
    struct handlePair
    {
      HANDLE m_hFile;
      HANDLE m_hMapping;
      unsigned char* m_ptrMapping;
      DWORD m_fileLength;
    };

  private:
    bool create(const std::string& fileName);
    bool start();
    bool wait();
    bool reap(const client&);
    bool report(const client&);

    std::list<client*> m_liClient;
    std::list<client*>::iterator m_itCurrent;
    std::list<client*>::iterator m_itEnd;

    std::map<std::string, handlePair> m_fileMap;

    unsigned int m_nDelay;
    unsigned int m_nRemaining;
    std::string m_hostName;
    unsigned short m_nPort;
    bool m_doRenew;

    // thread related, wait for client
    HANDLE m_condvarWaitForClient;
  };
}


#endif // ! ZT_SPOOL_HH
