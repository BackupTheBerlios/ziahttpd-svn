//
// server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 13:33:25 2006 texane
// Last update Sun Jan 22 17:47:18 2006 texane
//


#include <list>
#include <string>
#include <iostream>
#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>


using std::cout;
using std::endl;
using std::string;
using std::list;
using namespace status;


void net::server::reset()
{
  m_config = 0;
  m_ioman = 0;
  m_done = false;
}


net::server::server(config* config)
{
  reset();
  m_config = config;
}


net::server::server(char** av)
{
  reset();
  m_config = new config(av);
}


net::server::server(const string& confpath)
{
  reset();
  m_config = new config(confpath);
}


net::server::~server()
{
  delete m_config;
}


status::error net::server::process_requests()
{
  // Process all the requests
  list<config::protocol*>::iterator it;
  m_config->get_protocol(it);

  
  while (!m_config->end_protocol(it))
    {
      cout << "port: " << (*it)->port << endl;
      ++it;
    }

  while (m_done == false)
    {
    }
  ziafs_return_status( NOTIMPL );
}
