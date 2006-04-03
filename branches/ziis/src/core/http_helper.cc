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
  return true;
}

bool	net::split_accept_encoding(std::string& str, std::vector<std::string>& vec)
{
	stringmanager::split(str, ",", vec);
	return true;
}
