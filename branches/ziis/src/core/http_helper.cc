#include <http_helper.hh>


bool		net::generate_chunk_header(buffer& data, size_t sz, chunk_pos_t chunk)
{
	std::string	chunk_hex;

	data.clear();
	stringmanager::dec_to_hex((int)sz, chunk_hex);
	if (chunk != CHUNK_FIRST)
		data += "\r\n" + chunk_hex;
	else
		data += chunk_hex;
	data += "\r\n";
	if ((chunk == CHUNK_LAST))
		data += "\r\n";
	return true;
}