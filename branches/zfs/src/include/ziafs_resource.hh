//
// ziafs_resource.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 11:34:11 2006 texane
// Last update Fri Feb 17 13:49:55 2006 texane
//


#ifndef ZIAFS_RESOURCE_HH
# define ZIAFS_RESOURCE_HH


#include <string>
#include <sys/sysapi.hh>
#include <ziafs_buffer.hh>


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
    virtual e_error flush(sysapi::insock::handle_t&) = 0;
    virtual e_error flush(sysapi::file::handle_t&) = 0;
    virtual e_error generate(unsigned int&) = 0;
    virtual e_error size(unsigned int&) = 0;

    // Chunk/data modifications
    e_error prepend_header(buffer&);
    e_error alter_content(void (*)(buffer&, void*), void*);
    bool is_input() const;
    bool is_output() const;

//     // feed the resource, if supported
//     bool feed_input();
//     // in the case of get methods...


  protected:
    // implement the current chunk
    buffer data;

  private:
    e_omode omode;
    e_id id;
  };

  class manager
  {
  public:
    e_error factory_create(handle*&, e_id, e_omode, const std::string&);
    e_error factory_destroy(handle*);
  };
}


#endif // ! ZIAFS_RESOURCE_HH
