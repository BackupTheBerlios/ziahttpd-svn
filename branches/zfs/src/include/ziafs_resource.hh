//
// ziafs_resource.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 11:34:11 2006 texane
// Last update Fri Feb 17 23:07:24 2006 texane
//


#ifndef ZIAFS_RESOURCE_HH
# define ZIAFS_RESOURCE_HH


#include <string>
#include <sys/sysapi.hh>
#include <ziafs_buffer.hh>
#include <ziafs_thr.hh>


namespace resource
{
  // openimg mode
  typedef enum
    {
      O_INPUT = 0,
      O_OUTPUT
    } e_omode;

  // type of resource
  typedef enum
    {
      ID_FILE = 0,
      ID_PROCESS,
      ID_BYFLY
    } e_id;

  // io completion
  typedef enum
    {
      E_SUCCESS = 0,
      E_NOT_IMPL,
      E_OP_ERROR,
      E_NOT_SUPP,
      E_NOT_OPENED,
      E_ALREADY_GEN,
      E_UNKNOWN
    } e_error;


  // forward declaration
  class manager;

  class handle
  {
    friend class manager;

  public:
    handle();
    virtual ~handle() {}

    // interface
    // serve the resource current data
    virtual e_error flush_network(thr::pool::slot_t&, sysapi::insock::handle_t&) = 0;
    virtual e_error flush_disk(sysapi::file::handle_t&) = 0;
    virtual e_error flush_environ() = 0;
    virtual e_error flush_input(thr::pool::slot_t&, buffer&) = 0;
    virtual e_error generate(unsigned int&) = 0;
    virtual e_error size(unsigned int&) = 0;

    // Chunk/data modifications
    e_error prepend_header(buffer&);
    e_error alter_content(void (*)(buffer&, void*), void*);
    // type of resource
    bool is_input() const;
    bool is_output() const;

  protected:
    // implement the current chunk
    buffer data;

  private:
    e_omode omode;
    e_id id;
  };
}


namespace resource
{
  class byfly : public handle
  {
  public:
    // construction/destruction
    byfly(unsigned int);
    ~byfly();

    // interface implementation
    e_error flush_network(thr::pool::slot_t&, sysapi::insock::handle_t&);
    e_error flush_disk(sysapi::file::handle_t&);
    e_error flush_environ();
    e_error flush_input(thr::pool::slot_t&, buffer&);
    e_error generate(unsigned int&);
    e_error size(unsigned int&);

  private:
    bool generated;
    unsigned int err_code;
  };
}


namespace resource
{
  class file : public handle
  {
  public:
    // construction/destruction
    file(const std::string&, e_omode);
    ~file();

    // interface implementation
    e_error flush_network(thr::pool::slot_t&, sysapi::insock::handle_t&);
    e_error flush_disk(sysapi::file::handle_t&);
    e_error flush_environ();
    e_error flush_input(thr::pool::slot_t&, buffer&);
    e_error generate(unsigned int&);
    e_error size(unsigned int&);

  private:
    bool opened;
    bool generated;
    std::string file_path;
    sysapi::file::handle_t file_handle;
    unsigned long long file_size;
  };
}


namespace resource
{
  class manager
  {
  public:
    e_error factory_create(handle*&, e_id, e_omode, const std::string&);
    e_error factory_create(handle*&, e_id, e_omode, const unsigned int);
    e_error factory_destroy(handle*);
  };
}


#endif // ! ZIAFS_RESOURCE_HH
