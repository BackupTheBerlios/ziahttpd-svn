//
// http_helper.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Mar 22 19:39:32 2006 texane
// Last update Wed Mar 22 19:39:51 2006 texane
//


#ifndef HTTP_HELPER_HH
# define HTTP_HELPER_HH


#include "buffer.hh"

namespace resource { class handle; }
class IInput;
class IOutput;

namespace http_helper
{
  typedef enum
    {
      CHUNK_FIRST = 0,
      CHUNK_MIDDLE,
      CHUNK_LAST
    } chunk_pos_t;

  bool generate_chunk_header(buffer& data, size_t sz, chunk_pos_t chunk);
  resource::handle* create_resource(IInput&, const char*, IOutput&);
}


#endif // ! HTTP_HELPER_HH
