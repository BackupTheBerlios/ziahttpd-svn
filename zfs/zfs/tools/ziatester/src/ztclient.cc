//
// ztclient.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Dec 06 12:35:58 2005 texane
// Last update Wed Feb 01 19:06:17 2006 texane
//


#include <list>
#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <sysapi/sysapi.hh>
#include <ztclient.hh>
#include <ztspool.hh>


using std::ostringstream;
using std::string;
using std::list;
using std::cout;
using std::cerr;
using std::endl;
using std::setw;


void	zt::client::reset()
{
  m_spoolAddr = 0;
  m_done = false;
  m_renew = false;
  m_conf.clear();
  m_rqstStatus = NOTSTARTED;
  m_nread = 0;
}


zt::client::client(spool* spoolAddr,
		   const string& srvName,
		   unsigned short srvPort,
		   unsigned int nId,
		   const string& fileName,
		   unsigned char* ptrMapping,
		   DWORD fileLength,
		   bool renew)
{
  reset();

  m_spoolAddr = spoolAddr;
  m_srvName = srvName;
  m_srvPort = srvPort;
  m_renew = renew;
  m_fileName = fileName;
  m_nId = nId;
  m_ptrMapping = ptrMapping;
  m_fileLength = fileLength;
  
  m_condvarWaitToStart = CreateEvent(0,
				     TRUE, // Automatically reset event
				     FALSE, // Non signaled at creation time
				     0);
  if (m_condvarWaitToStart == NULL)
    sysapi::error::stringify("Cannot create condvar");
  sysapi::thread::create_and_exec(&m_thrHandle,
				  thrEntry,
				  reinterpret_cast<sysapi::thread::param_t>(this));
}


zt::client::~client()
{
  CloseHandle(m_condvarWaitToStart);
}


// Helper timing functions

static inline bool systemtimeDiffToMillisecond(unsigned long& res,
					       const SYSTEMTIME& a,
					       const SYSTEMTIME& b)
{
  unsigned long tmA = a.wSecond * 1000 + a.wMilliseconds;
  unsigned long tmB = b.wSecond * 1000 + b.wMilliseconds;
  res = tmA - tmB;
  return true;
}

static inline string millisecondToString(unsigned long mSec)
{
  ostringstream strm;
  strm << mSec / 1000 << "," << mSec % 1000 << " sec.";
  return strm.str();
}

void	zt::client::report() const
{
  unsigned long tmDiff;
  const char* rqstStatus;
  static unsigned int nr_not_connected = 0;

  systemtimeDiffToMillisecond(tmDiff, m_tmEnd, m_tmStart);
  switch (m_rqstStatus)
    {
    case NOTSTARTED:
      rqstStatus = "not_started";
      break;
    case NOTCONNECTED:
      ++nr_not_connected;
      rqstStatus = "not_connected";
      cout << "not connected: " << nr_not_connected << endl;
      break;
    case ABORTED:
      rqstStatus = "aborted";
      break;
    case COMPLETED:
      rqstStatus = "completed";
      break;
    default:
      rqstStatus = "unknown";
      break;
    }
  cout << "\t" << idToString() << " <" << setw(15) << rqstStatus << ">: " << millisecondToString(tmDiff) << ", " << m_nread << endl;
}


string	zt::client::toString() const
{
  return "";
}


string	zt::client::idToString() const
{
  ostringstream strm;
  strm << "[" << setw(5) << m_nId << "]";
  return strm.str();
}
