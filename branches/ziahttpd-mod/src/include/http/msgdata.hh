//
// msgdata.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 11 17:29:32 2005 texane
// Last update Sun Nov 13 16:04:05 2005 
//


#ifndef HTTP_MSGDATA_HH
# define HTTP_MSGDATA_HH


#include <string>
#include <map>
#include <http/uri.hh>
#include <dataman/buffer.hh>
#include <vector>

namespace http
{
  class msgdata
  {

  public:
    ~msgdata();

    std::string& operator[](const std::string&);
	std::string& operator=(const std::string&);
    bool parse_rqstline(dataman::buffer&, uri&);
    bool build_respline(msgdata& rqst, const uri&);
	bool stringify_respline(dataman::buffer& metada,  http::uri&);

	std::string& method_string()	{ return method_; };
	std::string& version_string()	{ return version_; };
	std::string& uri_string()		{ return uri_; };
	std::string& query_string();

	msgdata& operator=(msgdata&);
  private:
	  std::string								method_;
	  std::string								uri_;
	  std::string								version_;
	  std::map<std::string, std::string>		hdrlines_;
	  std::string								query_;

	  bool parse_rqstline_statusline(dataman::buffer& buf, uri&);
	  bool parse_rqstline_headerline(dataman::buffer& buf, uri&);
	  bool parse_uri(std::string &uri);
  };
}


//! \class http::msgdata
//! \brief Http messages data manipulation
//!
//! This class have 2 Functionality
//! first one is parse and store request from the client
//! second is build the response line (status line + header lines)
//! Module can access to the header lines for get or out information.

//! \fn std::string& http::msgdata::operator[](const std::string&)
//! \brief get string data from a key value 
//!
//! \param key value
//! \return string data

//! \fn bool http::msgdata::parse_rqstline(dataman::buffer&, uri&)
//! \brief parse the request lines( status line + headers lines )
//!
//! \param buffer header line
//! \param current uri 
//! \return if the parsing had failed
//! This function store all the information about the http protocol of the request

//! \fn bool http::msgdata::bool build_respline(msgdata& rqst, const uri&);
//! \brief build a default response header in a stringmap
//!
//! \param rqst current client request 
//! \param current uri
//! \return always true
//! This function build a default response metadata ( statusline + header lines ) in a Stringmap.
//! Module can get or put some entry

//! \fn bool http::msgdata::bool stringify_respline(dataman::buffer& metada)
//! \brief transform msgdata storage response on a buffer for send to client
//!
//! \param response buffer
//! \return always true
//! This function transform msgdata storage response on a buffer for send to client

#endif // !HTTP_MSGDATA_HH
