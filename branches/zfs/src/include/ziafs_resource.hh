//
// ziafs_resource.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 11:34:11 2006 texane
// Last update Fri Feb 17 14:39:54 2006 texane
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
      E_UNKNOWN
    } e_error;


//   {
//     // case of a 4 Go putted file??
//     factory_create(res);

//     if (request->content_length > static_limit(??))

//     // the process recieves input and
//     // env as parameters
//     // Get the size to send
//     if (res->input_resource())
//       {
// 	while (res->generate_content(&sz) == true)
// 	  {
// 	    res->alter(&new_sz);
// 	    // the first time, generate http header
// 	    // if chunked -> generate chunk header
// 	    http->create_header(hdr_buf, chunked);
// 	    res->prepend_header(hdr_buf);
// 	    res->flush();
// 	  }
//       }
//     else if (res->output_resrouce)
//       {
// 	while (get_next_body(buf) == true)
// 	  {
// 	    alter_body();
// 	    res->put();
// 	  }
//       }

//     factory_close(res);
//   }

  class handle
  {
  public:
    handle();
    virtual ~handle() {}

    // interface
    // serve the resource current data
    virtual e_error flush_network(thr::pool::slot_t&, sysapi::insock::handle_t&) = 0;
    virtual e_error flush_disk(sysapi::file::handle_t&) = 0;
    virtual e_error flush_environ() = 0;
    virtual e_error flush_input() = 0;
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
    e_error flush_input();
    e_error generate(unsigned int&);
    e_error size(unsigned int&);

  private:
    unsigned int err_code;
  };
}


namespace resource
{
  class manager
  {
  public:
    e_error factory_create(handle*&, e_id, e_omode, const std::string&);
    e_error factory_destroy(handle*);
  };
}


#endif // ! ZIAFS_RESOURCE_HH
