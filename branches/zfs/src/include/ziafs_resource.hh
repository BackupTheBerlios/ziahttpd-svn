//
// ziafs_resource.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Feb 17 11:34:11 2006 texane
// Last update Fri Feb 17 12:42:55 2006 texane
//


#ifndef ZIAFS_RESOURCE_HH
# define ZIAFS_RESOURCE_HH


#include <string>
#include <sys/sysapi.hh>


namespace resource
{
  // type of resource
  typedef enum
    {
      ID_FILE = 0,
      ID_PROCESS,
      ID_BYFLY
    } id_t;

  // io completion
  typedef enum
    {
    } e_compl_t;

  class manager
  {
  public:
//     static inf* factory_create(const std::string&);
//     static inf* factory_destroy(const std::string&);
  };


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
// 	    res->serve();
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

  class inf
  {
  public:
//     // creation
//     bool open();
//     bool close();

//     // sending
//     bool serve();

//     // resource informations
//     bool dynamic_content();
//     bool size();

//     // http headers
//     bool prepend_header();
//     bool prepend_chunk_header();

//     // apply some filter on the resource
//     bool alter_content();
//     // feed the resource, if supported
//     bool feed_input();
//     // in the case of get methods...
//     bool serve(sysapi::insock::handle_t&);
//     bool write();
  };
}


#endif // ! ZIAFS_RESOURCE_HH
