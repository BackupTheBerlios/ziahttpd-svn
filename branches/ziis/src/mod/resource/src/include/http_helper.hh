

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
