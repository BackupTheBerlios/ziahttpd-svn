//
// server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 13:33:25 2006 texane
// Last update Sat Feb 11 20:50:22 2006 
//


#include <list>
#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_status.hh>
#include <core/ziafs_debug.hh>
#include <sys/sysapi.hh>


// todo list
// + get resource from the connection module
// + integrate created connections to the iomanager
// + create a server for each port in the list(?).
// + add the resource to res_manager
// -> at creation time, place the resource into
// a list
// + add checks in add session


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


net::config* net::server::conf()
{
  return m_config;
}


io::res_manager* net::server::res_manager()
{
  return m_resman;
}


status::error net::server::add_session(session* sess)
{
  sess->m_server = this;
  m_sessions.push_front(sess);
  ziafs_return_status( SUCCESS );
}


status::error net::server::remove_session_byresource(io::resource* res)
{
  // Remove all the session whose
  // resource is res

  list<session*>::iterator cur = m_sessions.begin();
  list<session*>::iterator last = m_sessions.end();
  list<session*>::iterator prev;

  while (cur != last)
    {
      prev = cur;
      ++cur;
      if ((*prev)->m_client == res ||
	  (*prev)->m_target == res)
	{
	  remove_session(*prev);
	}
    }

  ziafs_return_status( SUCCESS );
}

status::error net::server::remove_session(session* sess)
{
  // Remove the given session
  ziafs_debug_msg("removing session %s\n", "");
  m_sessions.remove(sess);
  delete sess;
  ziafs_return_status( SUCCESS );
}


status::error net::server::process_requests()
{
  list<session*>::iterator cur;
  list<session*>::iterator last;
  list<config::protocol*>::iterator it;
  list<io::resource*> res_queue;
  io::resource* res;
  void* aux = reinterpret_cast<void*>(this);

  // Create a resource foreach port
  m_config->get_protocol(it);
  while (!m_config->end_protocol(it))
    {
      m_resman->create(res, io::ST_FETCHING, "localhost", 40000);
      m_resman->open(res);
      ziafs_print_object( *res );
      ++it;
    }

  // Serve incoming requests
  while (m_done == false)
    {
      ziafs_debug_msg("session count %d\n", m_sessions.size());

      // Get resources on which activity
      // has been seen.

      ziafs_debug_msg("reaping the resource %s\n", "");
      m_resman->reap_resources(this);

      m_resman->dispatch_io(res_queue, aux);

      while (res_queue.size())
	{
	  // Here try to match the session
	  // associated with the resource
	  // that has triggered activity.
	  res = res_queue.front();
	  res_queue.pop_front();
	  cur = m_sessions.begin();
	  last = m_sessions.end();
	  while (cur != last)
	    {
	      if ((*cur)->m_client == res ||
		  (*cur)->m_target == res)
		{
		  // Here should send a file
		  // to the client for btesting
		  // purposes

		  // Uncomment this one
		  (*cur)->process();
// 		  if ((*cur)->send_file() == status::SUCCESS)
// 		    ziafs_debug_msg("end of file reached %s!", "");
		}
	      ++cur;
	    }

	}
      
    }

  ziafs_return_status( SUCCESS );
}
