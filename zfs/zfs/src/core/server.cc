//
// server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 13:33:25 2006 texane
// Last update Thu Jan 26 22:38:29 2006 texane
//


#include <list>
#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_status.hh>
#include <sys/sysapi.hh>


// todo list
// + get resource from the connection module
// + integrate created connections to the iomanager


using std::string;
using std::list;
using namespace status;


void net::server::reset()
{
  m_config = 0;
  m_resman = 0;
  m_modman = 0;
  m_done = false;
}

void net::server::init()
{
  reset();
  sysapi::insock::init_subsystem();
}

void net::server::release()
{
  list<session*>::iterator current = m_sessions.begin();
  list<session*>::iterator last = m_sessions.end();

  // release all the session
  while (current != last)
    {
      // ! we should reap the resources
      // + iomanager here...
      delete *current;
      ++current;
    }

  if (m_resman)
    delete m_resman;
  if (m_config)
    delete m_config;
  if (m_modman)
    delete m_modman;

  sysapi::insock::release_subsystem();
}


// Exported functions

net::server::server(config* config)
{
  init();
  m_config = config;
  m_resman = new io::res_manager;
  m_modman = new mod_manager(config);
}


net::server::server(char** av)
{
  init();
  m_config = new config(av);
  m_resman = new io::res_manager;
  m_modman = new mod_manager(m_config);
}


net::server::server(const string& confpath)
{
  init();
  m_config = new config(confpath);
  m_resman = new io::res_manager;
  m_modman = new mod_manager(m_config);
}


net::server::~server()
{
  release();
}


status::error net::server::process_requests()
{
  list<config::protocol*>::iterator it;
  list<session*>::iterator sess_current;
  list<session*>::iterator sess_last;
  net::session* sess_client;
  net::protocol* proto_client;
  io::resource* res_serv;
  io::resource* res_client;

  // Create a resource foreach port
  m_config->get_protocol(it);
  while (!m_config->end_protocol(it))
    {
      res_serv = new io::res_insock(io::ST_FETCHING, "localhost", 40000);
      proto_client = new http;
      ziafs_print_object( *res_serv );
      ++it;
    }

  // Get incoming connections
  res_serv->io_on_open();
  res_serv->io_on_read((void*&)res_client);

  // Create a new session
  sess_client = new session(res_client, m_config, proto_client);
  m_sessions.push_front(sess_client);

  // Serve incoming requests
  while (m_done == false)
    {
      sess_current = m_sessions.begin();
      sess_last = m_sessions.end();
      while (sess_current != sess_last)
	{
	  (*sess_current)->process();
	  ++sess_current;
	}
    }

  ziafs_return_status( SUCCESS );
}
