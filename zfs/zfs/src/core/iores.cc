//
// resource.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 01:20:15 2006 texane
// Last update Sun Jan 22 16:44:38 2006 texane
//


#include <core/ziafs_io.hh>
#include <core/ziafs_status.hh>
#include <core/ziafs_buffer.hh>


using namespace io;
using namespace status;


// Helper functions

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


// Implementation

resource::resource(stmask openmod)
{
  m_pending = IO_NONE;
  m_state = ST_NONE;
  m_openmod = openmod;
  m_refcount = 0;
}


error resource::fetch(buffer& buf)
{
  if ((m_openmod & ST_FETCHING) == false)
    ziafs_return_status( BADMODE );

  // Fetch the user buffer
  setbit(m_state, ST_FETCHING);
  clrbit(m_pending, IO_READ);
  buf = m_fetch_buf;
  m_fetch_buf.reset();

  ziafs_return_status( SUCCESS );
}


error resource::feed(buffer& buf)
{
  if ((m_openmod & ST_FEEDING) == false)
    ziafs_return_status( BADMODE );

  // Tell the iomanager about
  // the feeding operation
  setbit(m_state, ST_FEEDING);
  setbit(m_pending, IO_WRITE);
  m_feed_buf = buf;
  buf.reset();

  ziafs_return_status( SUCCESS );
}
