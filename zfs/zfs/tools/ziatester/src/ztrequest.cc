//
// ztrequest.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Dec 06 22:41:13 2005 texane
// Last update Sun Feb 05 14:13:35 2006 texane
//


#include <string>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
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


bool	zt::client::connectServer()
{
  bool ret;
  ret = sysapi::socket_in::create_client(&m_sockHandle, m_srvPort, m_srvName.c_str());
  if (ret == false)
    {
      m_rqstStatus = NOTCONNECTED;
      // sysapi::error::stringify("Cannot connect to the server");
      return false;
    }
  return true;
}


bool	zt::client::closeServer()
{
  sysapi::socket_in::terminate_connection(m_sockHandle);
  m_rqstStatus = COMPLETED;
  return true;
}


bool	zt::client::sendToServer()
{
//   unsigned char* ptr;
  unsigned int nSent;
  bool done;
//   unsigned int nToSend;

//   nToSend = m_fileLength;
//   ptr = m_ptrMapping;
//   while (nToSend)
//     {
//       sysapi::socket_in::send(m_sockHandle,
// 			      ptr,
// 			      nToSend,
// 			      (sysapi::socket_in::size_t*)&nSent);
//       ptr += nToSend;
//       nToSend -= nSent;
//     }

  static char* megabuf = 0;
  static char* buf = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx2";
  static int sz = 0;
  char* ptr;
  int i;

  // Won't be freed
  if (megabuf == 0)
    {
      sz = ((int)strlen(buf) + 1) * 100;
      megabuf = (char*)malloc(sz);
      *megabuf = 0;
      for (i = 0; i < 100; ++i)
	strcat(megabuf, buf);
      sz = strlen(megabuf);
    }

  done = false;
  m_fileLength = 0;
  ptr = megabuf;
  while (done == false)
    {
      if (sysapi::socket_in::send(m_sockHandle,
				  (unsigned char*)ptr,
				  sz,
				  (sysapi::socket_in::size_t*)&nSent))
// 	done = true;
	;
      ptr += nSent;
      sz -= nSent;
//       if (sz <= 0)
// 	done = true;
      m_fileLength += nSent;
    }

  printf("nr sent == %d\n", m_fileLength);

  return true;
}


bool	zt::client::recvFromServer()
{
  // The server will disconnect you
  // when your inactivity time will
  // be too long

  bool done;
  sysapi::socket_in::size_t nread;
  sysapi::socket_in::size_t ntot;
  char buf[42];

  m_nread = 0;
  done = false;
  ntot = 0;
  while (done == false)
    {
      if (sysapi::socket_in::recv(m_sockHandle,
				  (unsigned char*)buf,
				  sizeof(buf),
				  &nread) == true)
	{
	  printf("ngot -- %d\n", nread);
	  ntot += nread;
	}
      else
	{
	  printf("closeed\n"); fflush(stdout);
	  done = true;
	}

      if (nread == 0)
	done = true;

      if (ntot >= m_fileLength)
	done = true;
    }

  m_nread = ntot;

  return true;
}


bool	zt::client::requestServer()
{
  if (connectServer() == false)
    return false;
  sendToServer();
  recvFromServer();
  closeServer();
  return true;
}
