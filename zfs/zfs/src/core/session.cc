//
// session.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 02:59:38 2006 texane
// Last update Sun Jan 22 13:53:27 2006 texane
//


#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_status.hh>


using namespace io;
using namespace net;


session::session(resource* client, config* config)
{
  m_client = client;
  m_config = config;
  m_target = 0;
  m_proto = 0;
}


session::~session()
{
  // Config is global, so
  // don't release it here.
  if (m_client)
    delete m_client;
  if (m_target)
    delete m_target;
  if (m_proto)
    delete m_proto;
}
