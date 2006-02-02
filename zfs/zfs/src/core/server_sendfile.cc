//
// server_sendfile.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Feb 01 17:57:06 2006 texane
// Last update Thu Feb 02 15:06:37 2006 texane
//


// This is a testing file


#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_debug.hh>
#include <core/ziafs_status.hh>


status::error net::session::send_file()
{
  // ? iores_file testing
  // This function create a new file
  // resource and send it to the client

  status::error err;
  buffer* buf;

  if (m_target == 0)
    {
      err = m_server->res_manager()->create(m_target, io::ST_FETCHING, "test\\article.asp.htm");
//       err = m_server->res_manager()->create(m_target, io::ST_FETCHING, "Makefile");
      err = m_server->res_manager()->open(m_target);
    }
  else
    {
      // fetch from the resource
      buf = 0;
      err = m_server->res_manager()->fetch(m_target, (void*&)buf);
      if (err == status::SUCCESS)
	m_server->res_manager()->feed(m_client, (void*&)buf);
      else
	m_server->res_manager()->close(m_target);
      if (buf != 0)
	delete buf;
    }

  ziafs_return_status( status::SUCCESS );
}
