#include <message.hh>

bool	http::message::error_code_string(std::string &dest, std::string &dest_file)
{
	struct	err_list_s
	{
		int			err_code;
		std::string str;
		std::string file;
	};
	err_list_s	err_list[] = 
	{
		{100, "Continue", "error.html"},	
		{101, "Switching Protocols", "error.html"},
		{200, "OK", "error.html"},
		{201, "Created", "error.html"},
		{202, "Accepted", "error.html"},
		{203, "Non-Authoritative Information", "error.html"},
		{204, "No Content", "error.html"},
		{205, "Reset Content", "error.html"},
		{206, "Partial Content", "error.html"},
		{300, "Multiple Choices", "error.html"},
		{301, "Moved Permanently", "error.html"},
		{302, "Found", "error.html"},
		{303, "See Other", "error.html"},
		{304, "Not Modified", "error.html"},
		{305, "Use Proxy", "error.html"},
		{307, "Temporary Redirect", "error.html"},
		{400, "Bad Request", "error.html"},
		{401, "Unauthorized", "error.html"},
		{402, "Payment Required", "error.html"},
		{403, "Forbidden", "error.html"},
		{404, "Not Found", "error.html"},
		{405, "Method Not Allowed", "error.html"},
		{406, "Not Acceptable", "error.html"},
		{407, "Proxy Authentication Required", "error.html"},
		{408, "Request Time-out", "error.html"},
		{409, "Conflict", "error.html"},
		{410, "Gone", "error.html"},
		{411, "Length Required", "error.html"},
		{412, "Precondition Failed", "error.html"},
		{413, "Request Entity Too Large", "error.html"},
		{414, "Request-URI Too Large", "error.html"},
		{415, "Unsupported Media Type", "error.html"},
		{416, "Requested range not satisfiable", "error.html"},
		{417, "Expectation Failed", "error.html"},
		{500, "Internal Server Error", "error.html"},
		{501, "Not Implemented", "error.html"},
		{502, "Bad Gateway", "error.html"},
		{503, "Service Unavailable", "error.html"},
		{504, "Gateway Time-out", "error.html"},
		{505, "HTTP Version not supported", "error.html"},
		{0, "", ""}
	};
	for (int i = 0; err_list[i].err_code; i++)
	{
		if (err_list[i].err_code == error_code_)
		{
			dest = err_list[i].str;
			dest_file = http::DOCROOT + err_list[i].file;
			return (true);
		}
	}
	return (false);
}
