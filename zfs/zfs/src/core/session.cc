//
// session.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 02:59:38 2006 texane
// Last update Sun Jan 29 18:01:17 2006 texane
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
  // The session hasnot to delete
  // the resource, since the io
  // manager will do so.
  // Just flag them as removed...

  if (m_client)
    m_client = 0;
  if (m_target)
    m_target = 0;
  if (m_proto)
    delete m_proto;
}


status::error session::process()
{
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
  m_proto->process_stage_fn(this);

  ziafs_return_status( status::SUCCESS );
}
