//
// ztspool.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Dec 06 12:34:59 2005 texane
// Last update Wed Dec 07 21:52:31 2005 texane
//


#include <map>
#include <list>
#include <string>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <sysapi/sysapi.hh>
#include <ztclient.hh>
#include <ztspool.hh>
#include <windows.h>


using std::map;
using std::list;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::setw;
using std::istringstream;


static bool	parseAv(const string& optEnt,
			unsigned int& nClient,
			string& fileName,
			bool& renew,
			map<string, zt::spool::handlePair>& fileMap)
{
  istringstream strm(optEnt);
  char skipMe;

  strm >> nClient >> skipMe >> fileName;
  renew = false;

  // If the mapping is not already done
  if (fileMap.find(fileName) == fileMap.end())
    {
      // Get the size of the file
      sysapi::file::size(fileName.c_str(), (unsigned long*)&fileMap[fileName].m_fileLength);

      // Create a new mapping for the file to send
      fileMap[fileName].m_hFile = CreateFile(fileName.c_str(),
					     GENERIC_READ,
					     FILE_SHARE_READ,
					     0,
					     OPEN_EXISTING,
					     FILE_ATTRIBUTE_READONLY,
					     0);
      if (fileMap[fileName].m_hFile == INVALID_HANDLE_VALUE)
	{
	  sysapi::error::stringify("Cannot open the file: ");
	  return false;
	}
      fileMap[fileName].m_hMapping = CreateFileMapping(fileMap[fileName].m_hFile,
						       0,
						       PAGE_READONLY,
						       0, 0,
						       0);
      if (fileMap[fileName].m_hMapping == INVALID_HANDLE_VALUE)
	{
	  sysapi::error::stringify("Cannot create the file mapping");
	  return false;
	}
      fileMap[fileName].m_ptrMapping = (unsigned char*)MapViewOfFile(fileMap[fileName].m_hMapping,
								     FILE_MAP_READ,
								     0, 0,
								     0);
      if (fileMap[fileName].m_ptrMapping == NULL)
	{
	  sysapi::error::stringify("Cannot map view of the file: ");
	  return false;
	}
    }

  return true;
}


static bool	parseAv(const string& optEnt,
			bool& doRenew,
			unsigned int& nDelay,
			unsigned short& nPort,
			string& hostName)
{
  // ?
  // Sorry, no time to spend out there...

  string optWrk = optEnt;

  if (!strncmp(optWrk.c_str(), "-delay:", strlen("-delay:")))
    {
      istringstream strm(optWrk.erase(0, strlen("-delay:")));
      strm >> nDelay;
    }
  else if (!strncmp(optWrk.c_str(), "-renew", strlen("-renew")))
    {
      istringstream strm(optWrk.erase(0, strlen("-renew")));
      doRenew = true;
    }
  else if (!strncmp(optWrk.c_str(), "-host:", strlen("-host:")))
    {
      istringstream strm(optWrk.erase(0, strlen("-host:")));
      strm >> hostName;
    }
  else if (!strncmp(optWrk.c_str(), "-port:", strlen("-port:")))
    {
      istringstream strm(optWrk.erase(0, strlen("-port:")));
      strm >> nPort;
    }

  return true;
}


zt::spool::spool(int ac, char** av, char**)
{
  unsigned int curId;
  unsigned int nClient;
  string fileName;
  bool renew;

  // Default those attributes
  m_hostName = "localhost";
  m_nPort = 40000;
  m_nRemaining = 0;
  m_nDelay = 0;
  m_doRenew = false;
  
  // First process command line args
  for (int i = 1; i < ac && *(av[i]) == '-'; ++i)
    {
      parseAv(av[i], m_doRenew, m_nDelay, m_nPort, m_hostName);
    }

  // Then process client creation
  curId = 0;
  for (; i < ac; ++i)
    {
      // Parse the command line entry
      parseAv(av[i], nClient, fileName, renew, m_fileMap);
      // Create nClient clients
      while (nClient > 0)
	{
	  m_liClient.push_back(new client(this,
					  m_hostName, m_nPort,
					  curId, fileName,
					  (unsigned char*)m_fileMap[fileName].m_ptrMapping,
					  m_fileMap[fileName].m_fileLength,
					  renew));
	  ++curId;
	  --nClient;
	}
    }
  m_itCurrent = m_liClient.begin();
  m_itEnd = m_liClient.end();

  // Threading synchronisation related
  m_condvarWaitForClient = CreateEvent(0,
				       TRUE,
				       FALSE,
				       0);
}


zt::spool::~spool()
{
  // ?
  // Close all mappings

  map<string, handlePair>::iterator itCur = m_fileMap.begin();
  map<string, handlePair>::iterator itEnd = m_fileMap.end();

  while (itCur != itEnd)
    {
      CloseHandle( ((*itCur).second).m_hMapping );
      CloseHandle( ((*itCur).second).m_hFile );
      ++itCur;
    }
}


bool	zt::spool::startNext(client*& res, client* toStart)
{
  if (toStart == 0)
    {
      if (m_itCurrent == m_itEnd)
	return false;
      toStart = *m_itCurrent;
      ++m_itCurrent;
    }

  cout << "\t" << idToString() << " <       starting>: (" << toStart->m_nId << ", " << toStart->m_fileName << ")" << endl;

  if (m_nDelay != 0)
    Sleep(m_nDelay);

  if (SetEvent(toStart->m_condvarWaitToStart) == FALSE)
    {
      sysapi::error::stringify("Cannot event");
      return false;
    }

  res = toStart;
  ++m_nRemaining;

  return true;
}


bool	zt::spool::waitNext(client*& res)
{
  // Wait for a client to signal me
  // This is a little bit tiring to explain, but
  // don't change the order in which those
  // functions are called...

  bool resetEvent = false;

 try_again:
  if (resetEvent == true)
    ResetEvent(m_condvarWaitForClient);
  list<client*>::iterator current = m_liClient.begin();
  list<client*>::iterator end = m_liClient.end();
  while (current != end)
    {
      if ((*current)->m_done == true && (*current)->m_renew == false)
	{
	  --m_nRemaining;
	  m_liClient.remove(*current);
	  res = *current;
	  return true;
	}
      ++current;
    }

  // Is there remaining clients?
  if (m_nRemaining == 0)
    return false;

  WaitForSingleObject(m_condvarWaitForClient, INFINITE);
  resetEvent = true;
  goto try_again;

  return false;
}


bool	zt::spool::handleEndofRequest(client* cli)
{
  if (cli->m_renew == false)
    {
      delete cli;
    }
  return true;
}


HANDLE	zt::spool::condvarWaitForClient() const
{
  return m_condvarWaitForClient;
}


string	zt::spool::idToString() const
{
  return "[Spool]";
}
