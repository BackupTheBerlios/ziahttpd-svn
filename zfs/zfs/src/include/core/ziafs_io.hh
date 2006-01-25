//
// ziafs_io.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:09:36 2006 texane
// Last update Wed Jan 25 14:00:59 2006 texane
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
  class res_manager;

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
      TYPEID_INSOCK_SSL,
      TYPEID_PROC
    } restype;


  // Resource base class
  class resource
  {
    friend class res_manager;

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
    status::error open();
    status::error close();
    status::error fetch(buffer&);
    status::error feed(buffer&);

    // Interface
    virtual status::error io_on_open() = 0;
    virtual status::error io_on_close() = 0;
    virtual status::error io_on_read(void*&) = 0;
    virtual status::error io_on_write() = 0;
    virtual status::error io_on_expire() = 0;
    virtual status::error io_has_expired(bool&) const = 0;
    virtual status::error dump(buffer&) const = 0;
    virtual status::error name(std::string&) const = 0;

    
  protected:
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
    status::error io_on_read(void*&);
    status::error io_on_write();
    status::error io_on_expire();
    status::error io_has_expired(bool&) const;
    status::error dump(buffer&) const;
    status::error name(std::string&) const;

  private:
    std::string m_path;
    unsigned int m_nrtoread;
    unsigned long m_filesz;
    sysapi::file::handle_t m_hfile;
  };
}


namespace io
{
  class res_insock : public resource
  {
  public:
    res_insock(stmask, const struct sockaddr_in&);
    res_insock(stmask, const std::string&, unsigned short);
    res_insock(stmask, const struct sockaddr_in&, const sysapi::insock::handle_t&);
    ~res_insock();

    // resouce interface implementation
    status::error io_on_open();
    status::error io_on_close();
    status::error io_on_read(void*&);
    status::error io_on_write();
    status::error io_on_expire();
    status::error io_has_expired(bool&) const;
    status::error dump(buffer&) const;
    status::error name(std::string&) const;

  private:
    // For accepting socket to be
    // bound at open time.
    bool m_accepting;
    std::string m_my_addr;
    unsigned short m_my_port;

    // socket for this connection part
    sysapi::insock::handle_t m_hsock;

    // inet addresses of the connection
    struct sockaddr_in m_local_addr;
    struct sockaddr_in m_foreign_addr;
  };
}


namespace io
{
  class res_manager
  {
  public:
    // Factory
    status::error create(resource*&, stmask, const std::string&);
    status::error destruct(resource*);

    status::error open(resource*);
    status::error close(resource*);
    status::error fetch(resource*);
    status::error feed(resource*);

  private:
    std::list<resource*> resources;
  };
}


#endif // ! ZIAFS_IO_HH
