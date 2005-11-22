//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/branches/ziahttpd-mod/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Nov 16 11:42:46 2005 
// Last update Tue Nov 22 08:18:26 2005 texane
//


// ?
// This module implement zia default behaviour regarding
// netword data handling (that's reading and writing).
// It should always be included in your server, unless you
// made a this-like module.


#include <zia.hh>
#include <list>
#include <string>
#include <cstring>
#include <iostream>


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::list;


// List of exported functions
MOD_EXPORT( HK_CREATE_CONNECTION )(http::session&, server::core*, int&);
MOD_EXPORT( HK_GET_RQST_METADATA )(http::session&, server::core*, int&);
MOD_EXPORT( HK_GET_RQST_DATA )(http::session&, server::core*, int&);
MOD_EXPORT( HK_SEND_RESPONSE)(http::session&, server::core*, int&);
MOD_EXPORT( HK_RELEASE_CONNECTION)(http::session&, server::core*, int&);


// Exported function definitions

MOD_EXPORT( HK_CREATE_CONNECTION )(http::session& session, server::core*, int&)
{
  // ?
  // Create connection.
  // Our basic iomanager is based on io multiplexing,
  // so this function needs to export a function to be called
  // at close time.

  debug_open(MODULE)
    << debug::setindent(4)
    << debug::setpunct('?')
    << "Creating new connection"
    << debug::fmt<debug::NEWLINE>
  debug_close()
    ;
  
  return true;
}


MOD_EXPORT( HK_GET_RQST_METADATA )(http::session& session, server::core*, int&)
{
  // ?
  // Read medata from the network.
  // For zia, meta data default reading function
  // reads http compliant lines from the network.

  sysapi::socket_in::error_t err;
  char* line;

  // Get the status line
  {
    if (dataman::get_nextline(session.hsock_con(), &line, &err) == false)
      {
	debug_open(MODULE)
	  << debug::setindent(4)
	  << debug::setpunct('!')
	  << "Cannot read metadata"
	  << debug::fmt<debug::NEWLINE>
	  debug_close()
	  ;

	return false;
      }

    dataman::buffer buffer(reinterpret_cast<const unsigned char*>(line), strlen(line));
    session.hdrlines_in().push_front(buffer);
    free(line);
  }

  // Read headerlines
  while (dataman::get_nextline(session.hsock_con(), &line, &err) && strlen(line))
    {
      dataman::buffer buffer(reinterpret_cast<unsigned char*>(line), strlen(line));
      session.hdrlines_in().push_back(buffer);
      free(line);
    }

  return true;
}


MOD_EXPORT( HK_GET_RQST_DATA )(http::session& session, server::core*, int&)
{
  // ?
  // Read data from the network.
  // For zia, data default reading function
  // reads http compliant lines from the network.

  unsigned char* content;
  sysapi::socket_in::size_t ncontent;

  debug_open(MODULE)
    << debug::setindent(4)
    << debug::setpunct('?')
    << "Requesting MetaData, Size: " << session.content_in().size() << debug::fmt<debug::NEWLINE>
  debug_close()
    ;
  
  if (session.content_in().size() == 0)
    return true;

  // Read request body
  {
    content = new unsigned char[session.content_in().size()];
    if (sysapi::socket_in::recv(session.hsock_con(), content, session.content_in().size(), &ncontent) == false)
      {
	debug_open(MODULE)
	  << debug::setindent(4)
	  << debug::setpunct('!')
	  << "Cannot read content"
	debug_close()
	  ;
	return false;
      }
  }
  
  // Fill in the content buffer
  {
    dataman::buffer buffer(content, ncontent);
    session.content_in() = buffer;
    delete[] content;
  }

  return true;
}


MOD_EXPORT( HK_SEND_RESPONSE)(http::session& session, server::core*, int&)
{
  // Display meta data
  {
    int i = 0;

    cout << "\t[<Default Module>] Response metadata" << endl;
    cout << "\t{" << endl;
    cout << "\t}" << endl;
  }

  // Send the response
  if (sysapi::socket_in::send(session.hsock_con(), (unsigned char*)session.hdrlines_out(), session.hdrlines_out().size()) == true &&
      sysapi::socket_in::send(session.hsock_con(), (unsigned char*)session.content_out(), session.content_out().size()) == true)
    {
      cout << "\t[ * ] From net module: Sending Success" << endl;
      return true;
    }

  cout << "\t[ * ] From net module: Sending Failure" << endl;
  return false;
}


MOD_EXPORT( HK_RELEASE_CONNECTION)(http::session& session, server::core*, int&)
{
  // ?
  // Relasing connection.
  // Our basic iomanager is based on io multiplexing,
  // so this function needs to export a function to be called
  // at close time.
    
  cout << "\t[<Default Module>] Releasing connection" << endl;
  sysapi::socket_in::terminate_connection(session.hsock_con());

  return true;
}
