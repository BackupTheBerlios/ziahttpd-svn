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
	static std::string header_key[] = {"Host", "Connection", ""};

	class	message 
	{
	public:
		//constructor 
		message(server::session&);
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
		int					error_code_;
		int					last_header_;

		class			statusline_c
		{
		public:
			int				method_;
			std::string		uri_;
			std::string		version_;
			std::string		query_;
			std::string		page_;
			std::map<std::string, std::string>  variables_;

			bool			method(const std::string &, int &err);
			bool			uri(const std::string &, int &err);
			bool			version(const std::string &, int &err);
		};

		class			header_c
		{
		public:
			std::map<std::string, std::string>  variables_;
		};

		statusline_c	statusline_data;
		header_c		header_data;
	};
};

#endif
