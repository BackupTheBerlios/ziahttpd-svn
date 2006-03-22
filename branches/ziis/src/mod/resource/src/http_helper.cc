#include "include/resource.hh"


using namespace std;


bool http_helper::generate_chunk_header(buffer& data, size_t sz, chunk_pos_t chunk)
{
  ostringstream oss;

  data.clear();
  oss << hex << (unsigned int)sz;

  if (chunk != CHUNK_FIRST)
    data += "\r\n" + oss.str();
  else
    data += oss.str();
  data += "\r\n";
  if ((chunk == CHUNK_LAST))
    data += "\r\n";
  return true;
}


resource::handle* http_helper::create_resource(IInput& in, const char* path, IOutput& out)
{
  return 0;
}
