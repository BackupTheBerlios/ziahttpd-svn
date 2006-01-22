//
// ziafs_io.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:09:36 2006 texane
// Last update Sun Jan 22 03:14:45 2006 texane
//


#ifndef ZIAFS_IO_HH
# define ZIAFS_IO_HH


#include <list>
#include <string>
#include <ziafs_buffer.hh>
#include <ziafs_status.hh>


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

  public:
    // construction/destruction
    resource();
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
    iomask m_pending;
    int m_refcount;
  };
}


// Concrete types
namespace io
{
  class res_file : public resource
  {
  public:
    ~res_file();
    status::error io_on_open();
    status::error io_on_close();
    status::error io_on_read();
    status::error io_on_write();
    status::error io_on_expire();
    status::error io_has_expired(bool&) const;
    status::error dump(buffer&) const;
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
