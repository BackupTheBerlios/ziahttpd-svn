//
// ztthread.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Dec 06 19:46:38 2005 texane
// Last update Wed Dec 07 16:25:41 2005 texane
//


#include <iostream>
#include <sysapi/sysapi.hh>
#include <ztspool.hh>
#include <ztclient.hh>


using std::cout;
using std::cerr;
using std::endl;


sysapi::thread::retcode_t zt::client::thrEntry(sysapi::thread::param_t param)
{
  client* thisClient = reinterpret_cast<zt::client*>(param);
  DWORD retVal;

  // Wait on the condition variable
  while ((retVal = WaitForSingleObject(thisClient->m_condvarWaitToStart, INFINITE)) == WAIT_OBJECT_0)
    {
      // Start the job
      ResetEvent(thisClient->m_condvarWaitToStart);
      thisClient->m_done = false;
      GetLocalTime(&thisClient->m_tmStart);
      thisClient->requestServer();
      GetLocalTime(&thisClient->m_tmEnd);

      // The client is done, for the spool to remove/restart it
      thisClient->m_done = true;

      // Tell the spool about the client status
      if (SetEvent(thisClient->m_spoolAddr->condvarWaitForClient()) == NULL)
	{
	  sysapi::error::stringify("error signaling the server");
	  return false;
	}

      // The client don't want to be restarted
      if (thisClient->m_renew == false)
	return true;
    }
  sysapi::error::stringify("opening the event:");
  return false;
}
