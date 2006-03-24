#include <ziafs_buffer.hh>
#include <zia_stringmanager.hh>

namespace net
{
	typedef enum
	{
		CHUNK_FIRST = 0,
		CHUNK_MIDDLE,
		CHUNK_LAST
	} chunk_pos_t;
	bool generate_chunk_header(buffer& data, size_t sz, chunk_pos_t chunk);
	bool split_accept_encoding(std::string&, std::vector<std::string>&);
}