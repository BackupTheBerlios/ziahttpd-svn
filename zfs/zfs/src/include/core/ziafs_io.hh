//
// ziafs_io.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:09:36 2006 texane
// Last update Sun Jan 22 16:46:21 2006 texane
//


#ifndef ZIAFS_IO_HH
# define ZIAFS_IO_HH


#include <list>
#include <string>
#include <sys/sysapi.hh>
#include <core/ziafs_buffer.hh>
#include <core/ziafs_status.hh>


namespace io
{
  // forward declaration
  class manager;

  // Creation mask
  typedef enum
    {
      ST_NONE	  = 0x00,
      ST_FETCHING = 0x01,
      ST_FEEDING  = 0x02
    } stmask;

  // Pending io bits
  typedef enum
    {
      IO_NONE    = 0x00,
      IO_READ    = 0x01,
      IO_WRITE   = 0x02,
      IO_CLOSE   = 0x04,
      IO_EXPIRED = 0x08      
    } iomask;

  // type of resource
  typedef enum
    {
      TYPEID_MEM = 0,
      TYPEID_FILE,
      TYPEID_INSOCK,
      TYPEID_PROC
    } restype;


  // Resource base class
  class resource
  {
    friend manager;

    // ?
    // resources are not created and opened
    // at the same time, since the protocol will
    // return a resource, and later the ioman
    // will do the actual open on it.
    // For instance, think about processs...

  public:
    // construction/destruction
    resource(stmask);
    virtual ~resource() {}

    // Same for all
    status::error fetch(buffer&);
    status::error feed(buffer&);

    // Interface
    virtual status::error io_on_open() = 0;
    virtual status::error io_on_close() = 0;
    virtual status::error io_on_read() = 0;
    virtual status::error io_on_write() = 0;
    virtual status::error io_on_expire() = 0;
    virtual status::error io_has_expired(bool&) const = 0;
    virtual status::error dump(buffer&) const = 0;

    
  private:
    // Manager related informations
    restype m_rtype;
    stmask m_state;
    stmask m_openmod;
    iomask m_pending;
    int m_refcount;
    buffer m_fetch_buf;
    buffer m_feed_buf;
  };
}


// Concrete types
namespace io
{
  class res_file : public resource
  {
  public:
    res_file(stmask, const std::string&);
    ~res_file();
    status::error io_on_open();
    status::error io_on_close();
    status::error io_on_read();
    status::error io_on_write();
    status::error io_on_expire();
    status::error io_has_expired(bool&) const;
    status::error dump(buffer&) const;

  private:
    // openmode_t m_omode;
    std::string m_path;
    unsigned int m_nrtoread;
    unsigned long m_filesz;
    sysapi::file::handle_t m_hfile;
  };
}


namespace io
{
  class manager
  {
  public:
    // Factory
    status::error create(resource*&, stmask, const std::string&);
    status::error destruct(resource*);

  private:
    std::list<resource*> resources;
  };
}


#endif // ! ZIAFS_IO_HH
