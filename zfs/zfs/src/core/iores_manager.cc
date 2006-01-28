//
// iores_manager.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 19:11:31 2006 texane
// Last update Sat Jan 28 17:47:12 2006 texane
//


#include <list>
#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_debug.hh>
#include <core/ziafs_status.hh>


// Todo list
// + check error code from io operation
// -> success, ok
// -> would block, don't make the resource expire...


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
  status::error err;
  fd_set rdset;
  fd_set wrset;
  res_insock* insock;
  bool activio;
  bool closeme;
  bool pushme;
  buffer* buf;
  int nev;

  // -
  // Prepare the sets
  FD_ZERO(&rdset);
  FD_ZERO(&wrset);
  cur = m_resources.begin();
  last = m_resources.end();
  while (cur != last)
    {
      if ((*cur)->m_typeid == io::TYPEID_INSOCK || (*cur)->m_typeid == io::TYPEID_INSOCK_SSL)
	{
	  insock = reinterpret_cast<res_insock*>(*cur);
 	  if (is_bitset(insock->m_pending, IO_READ) == true || insock->m_accepting == true)
	    {
	      ziafs_debug_msg("putting new resource in set %s\n", "");
	      // clrbit((*cur)->m_pending, IO_READ);
	      FD_SET(insock->m_hsock, &rdset);
	    }
	  if (is_bitset(insock->m_pending, IO_WRITE) == true)
	    FD_SET(insock->m_hsock, &wrset);
	}
      ++cur;
    }

  // -
  // Dispatch pending io

  ziafs_debug_msg("blocked in select %s\n", "");
  nev = select(0, &rdset, &wrset, 0, 0);
  ziafs_debug_msg("returned from select: %d\n", nev);
//   getchar();

  if (nev == -1)
    ziafs_return_status( FAILED );
  cur = m_resources.begin();
  last = m_resources.end();
  while (cur != last && nev > 0)
    {
      activio = true;
      closeme = false;
      pushme = false;
      insock = reinterpret_cast<res_insock*>(*cur);

      if (FD_ISSET(insock->m_hsock, &rdset))
	{
	  activio = true;
	  buf = &(*cur)->m_rd_buf;
	  err = insock->io_on_read((void*&)buf, aux);
	  if (err != status::SUCCESS)
	    closeme = true;
	  else
	    pushme = true;
	}

      if (FD_ISSET(insock->m_hsock, &wrset))
	{
	  buf = &(*cur)->m_wr_buf;
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
		  clrbit((*cur)->m_pending, IO_WRITE);
		}
	      else
		{
		  pushme = true;
		}
	    }
	}

      if (closeme == true)
	{
	  // mark the resource as having expired
	  insock->io_on_close();
	  insock->m_expired = true;
	}
      else if (pushme)
	{
	  q.push_front(*cur);
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
  res->m_pending = IO_READ;
  res->m_refcount = 1;
  m_resources.push_front(res);
  ziafs_return_status( status::SUCCESS );
}


status::error io::res_manager::create(resource*& res, stmask omask, const struct sockaddr_in& inaddr, const sysapi::insock::handle_t& hsrv)
{
  // Instanciate a client socket

  res = new res_insock(omask, inaddr, hsrv);
  res->m_pending = IO_READ;
  res->m_typeid = TYPEID_INSOCK;
  res->m_refcount = 1;
  m_resources.push_front(res);
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
  buffer* buf;

//   if ((res->m_openmod & ST_FETCHING) == false)
//     ziafs_return_status( BADMODE );

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
  // should get a list of sessions
  // that will be ready to be processed

  dispatch_socket_io(q, aux);
  ziafs_return_status( SUCCESS );
}


status::error io::res_manager::load_config(const net::config&)
{
  ziafs_return_status( NOTIMPL );
}
