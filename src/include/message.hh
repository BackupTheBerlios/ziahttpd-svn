/********************************************************************
	created:	2005/10/13
	created:	13:10:2005   16:55
	filename: 	e:\work\c++\zia3\message.hh
	file path:	e:\work\c++\zia3
	file base:	message
	file ext:	hh
	author:		
	
	purpose:	
*********************************************************************/


#ifndef		HTTP_MESSAGE_HH
#define		HTTP_MESSAGE_HH

#include <string>
#include <map>
#include <list>

namespace server {
class session;
}
#ifndef _TEST
#include <sysapi.hh>
#endif

namespace	http
{
	static std::string statusline_key[] = {"get", "post", "option", "head", "put", "delete", "trace", "connect", ""};

	class	message 
	{
	public:
		//constructor 
		message(server::session *);
		// check if the status line is correct
		bool			statusline(const std::string&);
		// check if the header is correct
		bool			header(const std::string&);
		// check if the body is correct ?
		bool			body(const unsigned char* data, size_t size);
		// make the response for the client
		bool			make_response();
	private:
		//current session
		server::session*	session_;
		//response error code if error .. if no error error_code = -1
		int					error_code_;
		// last header index
		int					last_header_;
		// method index
		std::string			method_;
		// complete uri
		std::string			uri_;
		// version of the http header
		std::string			version_;
		// status line query 
		std::string			query_;
		// what url the client want ?
		std::string			page_;
		// variable by GET method
		bool				page_is_dir;
		// full path of the DocumentRoot + file 
		std::string			file_;
		std::map<std::string, std::string>  getquery_;
		// variable by POST method
		std::map<std::string, std::string>  postquery_;
		// list of header fields
		//std::map<std::string, std::string>  header_;
		// status_line_string
		std::string					statusline_;
		std::list<std::string>		header_;

		bool			uri(const std::string &, int &err);
		bool			error_code_string(std::string &);
		bool			make_statusline(char *);
		bool			make_header(char *);
		bool			make_body(unsigned char *, sysapi::socket_in::size_t&);
		static bool		response_header_content_length(server::session *, const std::string&);
	};
}

#endif
