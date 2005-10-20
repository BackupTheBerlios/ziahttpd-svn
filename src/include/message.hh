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
#ifndef _TEST
#include <session.hh>
#endif
#ifdef _TEST

namespace server
{
	class session
	{
	public:
	protected:
	private:
	};
};
#endif

namespace	http
{
	static std::string statusline_key[] = {"GET", "POST", "OPTION", "HEAD", "PUT", "DELETE", "TRACE", "CONNECT", ""};

	class	message 
	{
	public:
		//constructor 
		message(server::session *);
		// check if the status line is correct
		bool			statusline(const std::string&);
		// build the status line for the client
		bool			statusline();
		// check if the header is correct
		bool			header(const std::string&);
		// build the header for the client
		bool			header();
		// check if the body is correct .... how i can know ?
		bool			body(const unsigned char*, size_t);
		// add the body at the end of the response for the client 
		bool			body();

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
		std::map<std::string, std::string>  getquery_;
		// variable by POST method
		std::map<std::string, std::string>  postquery_;
		// list of header fields
		std::map<std::string, std::string>  header_;

		bool			uri(const std::string &, int &err);
		bool			error_code_string(std::string &);
	};
};

#endif
