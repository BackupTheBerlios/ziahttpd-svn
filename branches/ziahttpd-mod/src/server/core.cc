//
// core.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Oct 11 21:28:14 2005 texane
// Last update Wed Nov 16 15:21:11 2005 
//


#include <string>
#include <iostream>
#include <server/core.hh>
#include <server/modhelper.hh>
#include <sysapi/sysapi.hh>
#include <dataman/conf.hh>
#include <http/session.hh>


using std::cout;
using std::cerr;
using std::endl;
using std::string;


// Singleton-like behaviour
server::core* server::core::instance_ = 0;
server::service* server::core::services_ = 0;


// - Construction, destruction

server::core::core()
{
  reset();
  sysapi::socket_in::init_subsystem();
  instance_ = this;
  services_ = new service;
  reload_conf();
}


server::core::core(int ac, char** av)
{
  reset();
  sysapi::socket_in::init_subsystem();
  instance_ = this;
  services_ = new service;
  reload_conf();
}


server::core::~core()
{
  sysapi::socket_in::release_subsystem();
}


bool	server::core::reset()
{
  services_ = 0;
  instance_ = 0;
  done_ = false;
  initialized_ = false;

  return true;
}


bool	server::core::release()
{
  return true;
}


bool	server::core::reload_conf()
{
  // Load modules
# if defined (_WIN32)
#	define MODNET	"modules\\net\\net.lo"
# else
#	define MODNET	"modules/net/net.lo"
# endif // _WIN32

  if (modman_.load_at_beginning(MODNET) == false)
    sysapi::error::stringify("ERROR LOADING MODULE");

  return true;
}


bool	server::core::reload_conf(const string& confname)
{
  return false;
}


bool	server::core::run()
{

  // !
  // This is a temporary version of the network engine.
  // One thread implement request handling, in order to begin
  // modularization of the request processing flow.

  // Create the server connection, accepting incoming ones
  http::session* session = new http::session(conf_);
  sysapi::socket_in::handle_t hsock_srv;
  sysapi::socket_in::handle_t hsock_con;
  sysapi::socket_in::create_listening(&hsock_srv, 40000);
  session->hsock_srv() = hsock_srv;
  sysapi::socket_in::accept(&hsock_con, hsock_srv);
  session->hsock_con() = hsock_con;

  // Create the request processing thread
  sysapi::thread::handle_t hthread;
  sysapi::thread::create_and_exec(&hthread, server::core::process_request, reinterpret_cast<sysapi::thread::param_t>(session));
  sysapi::thread::wait_single(hthread);
  sysapi::thread::release(hthread);

  // Delete the session
  delete session;

  return true;
}


server::core*	server::core::instance()
{
  return instance_;
}


sysapi::thread::retcode_t	server::core::process_request(sysapi::thread::param_t param)
{
  http::session* session = reinterpret_cast<http::session*>(param);

  cout << "[?] Entering processing function" << endl;

  try
    {
      // - Request reception part
      modman::instance()->call_hooks(core::instance(), modman::CREATE_CON, session);
      modman::instance()->call_hooks(core::instance(), modman::READ_RQST_METADATA, session);
      modman::instance()->call_hooks(core::instance(), modman::READ_RQST_DATA, session);
      modman::instance()->call_hooks(core::instance(), modman::PARSE_RQST_METADATA, session);
      modman::instance()->call_hooks(core::instance(), modman::ALTER_RQST_DATA, session);

      // - Response creation part
      modman::instance()->call_hooks(core::instance(), modman::BUILD_RESP_METADATA, session);
      modman::instance()->call_hooks(core::instance(), modman::BUILD_RESP_DATA, session);
      modman::instance()->call_hooks(core::instance(), modman::ALTER_RESP_DATA, session);
      modman::instance()->call_hooks(core::instance(), modman::ALTER_RESP_METADATA, session);
      modman::instance()->call_hooks(core::instance(), modman::SEND_RESP, session);
      modman::instance()->call_hooks(core::instance(), modman::RELEASE_CON, session);

    }
  catch (...)
    {
      cerr << "[!] Error occured in error processing" << endl;
    }

  cout << "[?] Returning from processing function" << endl;

  return 0;
}
