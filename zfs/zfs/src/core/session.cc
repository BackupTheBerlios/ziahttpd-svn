//
// session.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 02:59:38 2006 texane
// Last update Wed Jan 25 17:08:51 2006 texane
//


#include <iostream>
#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_utils.hh>
#include <core/ziafs_buffer.hh>
#include <core/ziafs_status.hh>


using namespace std;
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


status::error session::process()
{
  string ln;
  buffer* buf;
  bool has_expired;

  // Check if the resource has expired
  // This should be done by resman
  has_expired = false;
  m_client->io_has_expired(has_expired);
  if (has_expired == true)
    {
      ziafs_return_status( status::EXPIRED );
    }

  // Process the pending data
  m_client->io_on_read((void*&)buf);
  if (buf)
    {
      if (m_line.from_buffer(ln, *buf) == true)
	{
	  // a line has been found
	}
      // proto->consum(session, buffer);
      // session->target = new io::res...
      // cout << buf->tostring() << endl;
      delete buf;
    }

  // should close the session somewhere
  // res_client->io_on_close();

  ziafs_return_status( status::PARTIALIMPL );
}
