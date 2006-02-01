//
// ztrequest.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Dec 06 22:41:13 2005 texane
// Last update Wed Feb 01 19:15:41 2006 texane
//


#include <string>
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
  unsigned char* ptr;
  unsigned int nSent;
  unsigned int nToSend;

  nToSend = m_fileLength;
  ptr = m_ptrMapping;
  while (nToSend)
    {
      sysapi::socket_in::send(m_sockHandle,
			      ptr,
			      nToSend,
			      (sysapi::socket_in::size_t*)&nSent);
      ptr += nToSend;
      nToSend -= nSent;
    }

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

  done = false;
  ntot = 0;
  while (done == false)
    {
      if (sysapi::socket_in::recv(m_sockHandle,
				  (unsigned char*)buf,
				  sizeof(buf),
				  &nread) == true)
	ntot += nread;
      else
	done = true;

      if (nread == 0)
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
