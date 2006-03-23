#include <iostream>
#include <iomanip>
#include <sstream>
#include <http_helper.hh>


using namespace std;

bool		net::generate_chunk_header(buffer& data, size_t sz, chunk_pos_t chunk)
{
  std::ostringstream oss;

  data.clear();
  oss << hex << (unsigned int)sz;

  data = oss.str() + "\r\n";
  if ((chunk == CHUNK_LAST))
    data += "\r\n";
  return true;
}
