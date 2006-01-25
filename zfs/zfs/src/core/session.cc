//
// session.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 02:59:38 2006 texane
// Last update Wed Jan 25 23:54:37 2006 texane
//


#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_utils.hh>
#include <core/ziafs_buffer.hh>
#include <core/ziafs_status.hh>


using std::string;
using namespace io;
using namespace net;


session::session(resource* client, config* config, protocol* proto)
{
  m_client = client;
  m_config = config;
  m_proto = proto;
  m_target = 0;
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
  // m_proto->process_stage_fn(this);

  buffer* buf;
  bool has_expired;

  // Check if the resource has expired
  // This should be done by resman
  has_expired = false;
  m_client->io_has_expired(has_expired);
  if (has_expired == true)
    {
      m_client->io_on_expire();
      ziafs_return_status( status::EXPIRED );
    }

  // Process the pending data
  if (m_client->io_on_read((void*&)buf) == status::SUCCESS)
    {
      if (buf)
	{
	  m_proto->consume(this, *buf);
	  // proto->consum(session, buffer);
	  // session->target = new io::res...
	  // cout << buf->tostring() << endl;
	  delete buf;
	}
    }

  // should close the session somewhere
  // res_client->io_on_close();

  ziafs_return_status( status::SUCCESS );
}
