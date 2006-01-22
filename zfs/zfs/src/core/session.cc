//
// session.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 02:59:38 2006 texane
// Last update Sun Jan 22 03:04:03 2006 texane
//


#include <ziafs_io.hh>
#include <ziafs_net.hh>
#include <ziafs_status.hh>


using namespace io;
using namespace net;


session::session(resource* client)
{
  m_client = 0;
  m_target = 0;
  m_proto = 0;
}


session::~session()
{
  if (m_client)
    delete m_client;
  if (m_target)
    delete m_target;
  if (m_proto)
    delete m_proto;
}
