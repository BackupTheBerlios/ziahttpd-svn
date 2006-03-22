//
// resource.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 10:25:56 2006 texane
// Last update Wed Mar 22 16:52:26 2006 texane
//


#ifndef RESOURCE_HH
# define RESOURCE_HH


#include <string>
#include <sys/sysapi.hh>
#include <ziis.hh>
#include "include/buffer.hh"


namespace resource
{
  // openimg mode
  typedef enum
    {
      O_INPUT = 0,
      O_OUTPUT,
      O_BOTH
    } e_omode;

  // type of resource
  typedef enum
    {
      ID_FILE = 0,
      ID_PROCESS,
      ID_BYFLY,
      ID_FAKE
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
      E_WOULDBLOCK,
      E_CONTINUE,
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
    virtual e_error flush_network(IOutput&) = 0;
    virtual e_error flush_disk(sysapi::file::handle_t&) = 0;
    virtual e_error flush_environ() = 0;
    virtual e_error flush_input(buffer&) = 0;
    virtual e_error generate(unsigned int&) = 0;
    virtual e_error size(unsigned int&) = 0;
    virtual bool is_content_dynamic() const = 0;

    // Chunk/data modifications
    e_error prepend_header(buffer&);
    e_error alter_content(void (*)(buffer&, void*), void*);
    // type of resource
    bool is_input() const;
    bool is_output() const;
    unsigned int input_size();
    bool is_prefetched_input();
    void get_prefetched_input(buffer&);

  protected:
    // implement the current chunk
    buffer data;

    // This is filled at create time
    buffer in_buf;
    // remaining size
    unsigned int in_size;
    // open mode
    e_omode omode;

  private:
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
    e_error flush_network(IOutput&);
    e_error flush_disk(sysapi::file::handle_t&);
    e_error flush_environ();
    e_error flush_input(buffer&);
    e_error generate(unsigned int&);
    e_error size(unsigned int&);
    bool is_content_dynamic() const;

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
    e_error flush_network(IOutput&);
    e_error flush_disk(sysapi::file::handle_t&);
    e_error flush_environ();
    e_error flush_input(buffer&);
    e_error generate(unsigned int&);
    e_error size(unsigned int&);
    bool is_content_dynamic() const;

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
  class process : public handle
  {
  public:
    // construction/destruction
    process(int, char**, char**, e_omode);
    ~process();

    // interface implementation
    e_error flush_network(IOutput&);
    e_error flush_disk(sysapi::file::handle_t&);
    e_error flush_environ();
    e_error flush_input(buffer&);
    e_error generate(unsigned int&);
    e_error size(unsigned int&);
    bool is_content_dynamic() const;

  private:
    sysapi::process::handle_t proc_handle;
    sysapi::file::handle_t read_handle;
    sysapi::file::handle_t write_handle;
    bool generated;

  };
}


namespace resource
{
  class fake : public handle
  {
  public:
    // construction/destruction
    fake();
    ~fake();

    // interface implementation
    e_error flush_network(IOutput&);
    e_error flush_disk(sysapi::file::handle_t&);
    e_error flush_environ();
    e_error flush_input(buffer&);
    e_error generate(unsigned int&);
    e_error size(unsigned int&);
    bool is_content_dynamic() const;

  private:
    bool generated;
  };
}


namespace resource
{
  class manager
  {
  public:
    static e_error factory_create(handle*&, e_id, e_omode, int, char**, char**, buffer* = 0, unsigned int = 0);
    static e_error factory_create(handle*&, e_id, e_omode, const std::string&, buffer* = 0, unsigned int = 0);
    static e_error factory_create(handle*&, e_id, e_omode, const unsigned int);
    static e_error factory_create(handle*&, e_id);
    static e_error factory_destroy(handle*);
  };
}



#endif // ! RESOURCE_HH
