//
// iores_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 19:11:31 2006 texane
// Last update Sat Feb 11 19:28:53 2006 texane
//


#include <list>
#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_net.hh>
#include <core/ziafs_debug.hh>
#include <core/ziafs_status.hh>


// Todo list
// + check error code from io operation
// -> success, ok
// -> would block, don't make the resource expire...
// + reque should be in the iomanager, so that it
// can check if present are valid or not (in the resource)
// set
// + sessions should be removed by the server,
// so code a function that does the removal by the
// server, according to a resource address


using std::list;
using std::string;
using namespace status;


// Internal management routines

template<typename T>
inline static void setbit(T& val, T bit)
{
  val = (T)((unsigned int)val | (unsigned int)bit);
}

template<typename T>
inline static void clrbit(T& val, T bit)
{
  val = (T)((unsigned int)val & ~((unsigned int)bit));
}

template<typename T>
inline static bool is_bitset(T val, T bit)
{
  return ((unsigned int)val & (unsigned int)bit) ? true : false;
}


status::error io::res_manager::dispatch_file_io(list<resource*>& q, void*& aux)
{
  buffer* buf;
  list<resource*>::iterator cur;
  list<resource*>::iterator last;
  status::error err;

  cur = m_resources.begin();
  last = m_resources.end();
  while (cur != last)
    {
      if ((*cur)->m_typeid == io::TYPEID_FILE)
	{
	  // Process write operations first
	  if (is_bitset((*cur)->m_pending, IO_WRITE) == true)
	    {
	      buf = &(*cur)->m_wr_buf;
	      err = (*cur)->io_on_write((void*&)buf, aux);
	    }

	  // Read operation
	  buf = &(*cur)->m_rd_buf;
	  err = (*cur)->io_on_read((void*&)buf, aux);
	  if (err != status::SUCCESS)
	    close(*cur);
	}
      ++cur;
    }

  ziafs_return_status( status::SUCCESS );  
}


status::error io::res_manager::dispatch_socket_io(list<resource*>& q, void*& aux)
{
  // Important notes:
  // . this is not important to read n bytes, just read
  // . for writting, we should clear the bit when the byte
  // count  has been written, allowing the user to advance
  // in its execution flow, assuming its buffer has tottally
  // been written.

  list<resource*>::iterator cur;
  list<resource*>::iterator last;
  list<resource*>::iterator prev;
  status::error err;
  fd_set rdset;
  fd_set wrset;
  int nfds;
  res_insock* insock;
  bool activio;
  bool closeme;
  bool pushme;
  buffer* buf;
  int nev;
  unsigned long long tm_current;

  sysapi::time::current(tm_current);

  // -
  // Prepare the sets
  FD_ZERO(&rdset);
  FD_ZERO(&wrset);
  nfds = 0;
  cur = m_resources.begin();
  last = m_resources.end();
  while (cur != last)
    {
      if ((*cur)->m_typeid == io::TYPEID_INSOCK || (*cur)->m_typeid == io::TYPEID_INSOCK_SSL)
	{
	  pushme = false;
	  insock = reinterpret_cast<res_insock*>(*cur);
 	  if (is_bitset(insock->m_pending, IO_READ) == true || insock->m_accepting == true)
	    {
	      FD_SET(insock->m_hsock, &rdset);
	      pushme = true;
	    }
	  if (is_bitset(insock->m_pending, IO_WRITE) == true)
	    {
	      FD_SET(insock->m_hsock, &wrset);
	      pushme = true;
	    }
	  if (insock->m_tm_lastio == 0)
	    insock->m_tm_lastio = tm_current;
	  if (pushme == true && insock->m_hsock > (unsigned int)nfds)
	    nfds = (unsigned int)insock->m_hsock;
	}
      ++cur;
    }

  // -
  // Dispatch pending io

#ifndef NetBSD
  nev = select(0, &rdset, &wrset, 0, 0);
#else
  nev = select(nfds + 1, &rdset, &wrset, 0, 0);
#endif // !NetBSD

  if (nev == -1)
    ziafs_return_status( FAILED );

  // here reap all dead resources
  // so that we can accept connections
  reap_resources(aux);

  cur = m_resources.begin();
  last = m_resources.end();
  while ((cur != last) && nev > 0)
    {
      activio = false;
      closeme = false;
      pushme = false;
      insock = reinterpret_cast<res_insock*>(*cur);

      prev = cur;
      ++cur;

      if (FD_ISSET(insock->m_hsock, &rdset))
	{
	  activio = true;
	  buf = &(*prev)->m_rd_buf;
	  err = insock->io_on_read((void*&)buf, aux);
	  if (err != status::SUCCESS)
	    closeme = true;
	  else
	    pushme = true;
	}

      if (FD_ISSET(insock->m_hsock, &wrset))
	{
	  buf = &(*prev)->m_wr_buf;
	  activio = true;
	  err = insock->io_on_write((void*&)buf, aux);
	  if (err != status::SUCCESS)
	    {
	      // The resource has expired
	      closeme = true;
	    }
	  else
	    {
	      if (buf->size())
		{
		  // There are no more data to write
		  clrbit((*prev)->m_pending, IO_WRITE);
		}
	      else
		{
		  pushme = true;
		}
	    }
	}

      if (activio == true)
	{
	  --nev;

	  // Set the last io field to this resource
	  insock->m_tm_lastio = tm_current;
	}

      if (closeme == true)
	close(*prev);
      else if (pushme == true)
	q.push_front(*prev);
    }

  ziafs_return_status( status::SUCCESS );
}




// Exported

io::res_manager::res_manager()
{
  m_nr_connections = NR_MAX_CONNECTIONS;
}


io::res_manager::~res_manager()
{
}


void io::res_manager::reset_resource(resource* res)
{
  res->m_typeid = TYPEID_UNKOWN;
  res->m_pending = IO_NONE;
  res->m_state = ST_NONE;
  res->m_openmod = ST_NONE;
  res->m_refcount = 0;
  res->m_expired = false;
  res->m_opened = false;
}


status::error io::res_manager::create(resource*& res, stmask omask, const std::string& local_addr, unsigned short local_port)
{
  // Instanciate an accepting socket

  res = new res_insock(omask, local_addr, local_port);
  reset_resource(res);
  res->m_typeid = TYPEID_INSOCK;
  res->m_pending = IO_READ;
  setbit(res->m_openmod, ST_FETCHING);
  m_resources.push_front(res);
  ziafs_return_status( status::SUCCESS );
}


status::error io::res_manager::create(resource*& res, stmask omask, const struct sockaddr_in& inaddr, const sysapi::insock::handle_t& hsrv)
{
  // Instanciate a client socket
  res = new res_insock(omask, inaddr, hsrv);
  reset_resource(res);
  res->m_pending = IO_READ;
  setbit(res->m_openmod, ST_FETCHING);
  setbit(res->m_openmod, ST_FEEDING);
  res->m_typeid = TYPEID_INSOCK;
  m_resources.push_front(res);
  ziafs_return_status( status::SUCCESS );
}


status::error io::res_manager::create(resource*& res, stmask omask, const std::string& path)
{
  res = new res_file(omask, path);
  reset_resource(res);
  setbit(res->m_openmod, ST_FETCHING);
  res->m_typeid = TYPEID_FILE;
  m_resources.push_front(res);
  ziafs_return_status( status::SUCCESS );
}


status::error io::res_manager::open(resource* res)
{
  // Call the resource open method
  // Mark the resource as opened
  // There should be io_add_manager method

  if (res->m_opened == false)
    {
      res->io_on_open();
      res->m_opened = true;
    }
  ++res->m_refcount;
  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::close(resource* res)
{
  // This is the only part in
  // the code that should release
  // resources.

  if (res->m_opened == false)
    ziafs_return_status( ISNT_OPENED );

  --res->m_refcount;
  if (res->m_refcount == 0)
    res->m_opened = false;

  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::fetch(resource* res, void*& pdata)
{
  buffer* buf;

  if (res->m_opened == false)
    ziafs_return_status( ISNT_OPENED );

  if ((res->m_openmod & ST_FETCHING) == false)
    ziafs_return_status( BADMODE );

  // Fetch the user buffer
  setbit(res->m_state, ST_FETCHING);
  buf = new buffer(res->m_rd_buf);
  pdata = buf;
  res->m_rd_buf.reset();

  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::feed(resource* res, void*& pdata)
{
  buffer* buf = (buffer*&)pdata;

  if (res->m_opened == false)
    ziafs_return_status( ISNT_OPENED );

  if ((res->m_openmod & ST_FEEDING) == false)
    ziafs_return_status( BADMODE );

  // Tell the iomanager about
  // the feeding operation
  setbit(res->m_state, ST_FEEDING);
  setbit(res->m_pending, IO_WRITE);
  res->m_wr_buf += *buf;

  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::dispatch_io(list<resource*>& q, void*& aux)
{
  // First process all the close requests
  // Dispatch io operation on sockets
  dispatch_socket_io(q, aux);
  dispatch_file_io(q, aux);
  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::reap_resources(void* aux)
{
  // !
  // At this point, its safe to do
  // a lot of things concerning resource
  // releasing.
  // DO ALL THIS PART HERE, don't spread
  // code that release resource, remove session..
  net::server* srv;
  bool has_expired;
  bool to_reap;
  list<resource*>::iterator cur_res = m_resources.begin();
  list<resource*>::iterator last_res = m_resources.end();
  list<resource*>::iterator prev_res;
  list<net::session*>::iterator cur_sess;
  list<net::session*>::iterator last_sess;
  list<net::session*>::iterator prev_sess;

  srv = (net::server*)aux;
  while (cur_res != last_res)
    {
      to_reap = false;
      prev_res = cur_res;
      ++cur_res;

      // Do we have to reap the resource?
      if ((*prev_res)->m_refcount == 0)
	{
	  to_reap = true;
	}
      else
	{
	  (*prev_res)->io_has_expired(has_expired);
	  if (has_expired == true)
	    to_reap = true;
	}

      // Reap the resource
      if (to_reap == true)
	{
	  // remove closed resources
	  cur_sess = srv->m_sessions.begin();
	  last_sess = srv->m_sessions.end();

	  // Here is a leak -> cannot delete
	  // between m_target and m_clients....

	  // remove associated sessions
	  while (cur_sess != last_sess)
	    {
	      prev_sess = cur_sess;
	      ++cur_sess;
	      if ((*prev_sess)->m_client == *prev_res ||
		  (*prev_sess)->m_target == *prev_res)
		{
		  delete *prev_sess;
		  srv->m_sessions.erase(prev_sess);
		}
	    }
	  (*prev_res)->io_on_close(aux);
	  delete *prev_res;
	  m_resources.erase(prev_res);
	}
    }
  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::load_config(const net::config&)
{
  ziafs_return_status( NOTIMPL );
}
