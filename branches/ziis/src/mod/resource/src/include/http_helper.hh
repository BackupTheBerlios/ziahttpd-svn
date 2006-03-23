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
#include "config.hh"

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
	enum resouce_type_t
	{
		IS_NONE,
		IS_FILE,
		IS_CGI,
		IS_PUT,
		EXEC_BY_CGI,
		EXEC_DIRECTORY_LISTING,
		IS_FLY,
		IS_FAKE
	};
  bool generate_chunk_header(buffer& data, size_t sz, chunk_pos_t chunk);
	resource::handle* create_resource(IInput&, const char*, IOutput&, net::config*);
	bool get_type_of_resource(net::config& conf, 
														resouce_type_t& type_r, 
														IInput& inp, 
														IOutput& out, 
														std::string& localname,
														int& status_code);
	std::string extension(std::string& localname);
	bool pre_create_resource(net::config& conf,
														resouce_type_t& r_type,
														int& status_code,
														IInput& inp,
														IOutput& out,
														std::string& localname);
	bool create_resource(resource::handle*& hld, 
												net::config& conf, IInput& inp, 
												IOutput& out, 
												std::string localname);
	bool get_cgi_path(net::config& conf, std::string& path, std::string& localname);
	bool genete_type_mimes(net::config& conf, IOutput& out, std::string& localname);
}


#endif // ! HTTP_HELPER_HH
