//
// iores_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 19:11:31 2006 texane
// Last update Thu Jan 26 21:59:01 2006 texane
//


#include <list>
#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_status.hh>


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


status::error io::res_manager::dispatch_socket_io()
{
  list<resource*>::iterator cur;
  list<resource*>::iterator last;
  fd_set rdset;
  fd_set wrset;
  res_insock* insock;
  bool activio;
  bool closeme;
  buffer* fake;
  int nev;

  // -
  // Prepare the sets
  FD_ZERO(&rdset);
  FD_ZERO(&wrset);
  cur = m_resources.begin();
  last = m_resources.end();
  while (cur != last)
    {
      insock = reinterpret_cast<res_insock*>(*cur);
      if (is_bitset(insock->m_pending, IO_READ) == true)
	FD_SET(insock->m_hsock, &rdset);
      if (is_bitset(insock->m_pending, IO_WRITE) == true)
	FD_SET(insock->m_hsock, &wrset);
      ++cur;
    }

  // -
  // Dispatch pending io

  nev = select(0, &rdset, &wrset, 0, 0);
  if (nev == -1)
    ziafs_return_status( FAILED );
  cur = m_resources.begin();
  last = m_resources.end();
  while (cur != last && nev > 0)
    {
      activio = true;
      closeme = false;
      insock = reinterpret_cast<res_insock*>(*cur);

      if (FD_ISSET(insock->m_hsock, &rdset))
	{
	  activio = true;
	  if (insock->io_on_read((void*&)fake) != status::SUCCESS)
	    closeme = true;
	}

      if (FD_ISSET(insock->m_hsock, &wrset))
	{
	  activio = true;
	  if (insock->io_on_write((void*&)fake) != status::SUCCESS)
	    closeme = true;
	}

      if (closeme == true)
	{
	  // mark the resource as having expired
	  insock->io_on_close();
	  insock->m_expired = true;
	}

      if (activio == true)
	--nev;
    }

  ziafs_return_status( status::SUCCESS );
}




// Exported

io::res_manager::res_manager()
{
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
}


status::error io::res_manager::create(resource*& res, stmask omask, const std::string& local_addr, unsigned short local_port)
{
  // Instanciate an accepting socket

  res = new res_insock(omask, local_addr, local_port);
  res->m_typeid = TYPEID_INSOCK;
  res->m_refcount = 1;
  
  ziafs_return_status( status::SUCCESS );
}


status::error io::res_manager::create(resource*& res, stmask omask, const struct sockaddr_in& inaddr, const sysapi::insock::handle_t& hsrv)
{
  // Instanciate a client socket

  res = new res_insock(omask, inaddr, hsrv);
  res->m_typeid = TYPEID_INSOCK;
  res->m_refcount = 1;

  ziafs_return_status( status::SUCCESS );
}


status::error io::res_manager::create(resource*&, stmask, const std::string&)
{
  ziafs_return_status( status::NOTIMPL );
}


status::error io::res_manager::destruct(resource*)
{
  ziafs_return_status( status::NOTIMPL );
}


status::error io::res_manager::open(resource* res)
{
  // Call the resource open method
  // Mark the resource as opened
  // There should be io_add_manager method

  res->m_opened = true;

  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::close(resource*)
{
  ziafs_return_status( NOTIMPL );
}


status::error io::res_manager::fetch(resource* res, void*& pdata)
{
  buffer* buf = (buffer*&)pdata;

  if ((res->m_openmod & ST_FETCHING) == false)
    ziafs_return_status( BADMODE );

  // Fetch the user buffer
  setbit(res->m_state, ST_FETCHING);
  clrbit(res->m_pending, IO_READ);
//   buf = res->m_rdbuf;
//   res->m_rdbuf.reset();

  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::feed(resource* res, void*& pdata)
{
  buffer* buf = (buffer*&)pdata;

  if ((res->m_openmod & ST_FEEDING) == false)
    ziafs_return_status( BADMODE );

  // Tell the iomanager about
  // the feeding operation
  setbit(res->m_state, ST_FEEDING);
  setbit(res->m_pending, IO_WRITE);
//   m_feed_buf = buf;
//   buf.reset();

  // Set the resource write buffer to...

  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::dispatch_io()
{
  dispatch_socket_io();
  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::load_config(const net::config&)
{
  ziafs_return_status( NOTIMPL );
}
