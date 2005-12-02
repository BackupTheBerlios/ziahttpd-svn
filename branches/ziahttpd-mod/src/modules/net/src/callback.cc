//
// callback.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Nov 22 19:44:26 2005 texane
// Last update Fri Dec 02 15:43:01 2005 texane
//


#include <string>
#include <cstring>
#include <iostream>
#include <sstream>
#include <zia.hh>
#include <callback.hh>


using std::hex;
using std::ostringstream;
using std::string;


// This file export callbacks to be
// called by server on events.
// The request processing has different
// stages, so we define several callback
// for {in, out} put operations.


extern server::service* services_;


bool read_metadata(sysapi::socket_in::handle_t& hsock,
		   dataman::buffer*,
		   sysapi::socket_in::error_t& reason)
{
  bool ret;
  char* line;
  http::session* session;
  sysapi::socket_in::error_t err;

  // Get the session address
  ret = services_->find_session_byid(hsock, session);
  if (ret == false)
    cout << "Cannot get the session, will segfault" << endl;

  { // Debugging purposes
    cout << "\t[?: " << std::dec << (unsigned int)session->hsock_con() << " ] Metadata Reading callback" << endl;
  }

  // Read the next http line
  while ((ret = dataman::get_nextline(hsock, &line, &err)) == true)
    {
      if (strlen(line) != 0)
	{
	  // It is a header line
	  dataman::buffer buffer(reinterpret_cast<const unsigned char*>(line), strlen(line));
	  session->hdrlines_in().push_back(buffer);
	  cout << "\t\t[+] hdrline: " << line << endl;
	}
      else
	{
	  // We are done with the
	  // metadata reading stage
	  session->services_->next_processing_stage(*session);
	  free(line);
	  return true;
	}
      free(line);
    }

  reason = err;
  return false;
}


bool read_data(sysapi::socket_in::handle_t& hsock,
	       dataman::buffer*,
	       sysapi::socket_in::error_t& reason)
{
  unsigned char* content;
  sysapi::socket_in::size_t nrecv;
  sysapi::socket_in::error_t err;
  http::session* session;
  bool ret;

  // Get the session address, gore mode
  ret = services_->find_session_byid(hsock, session);
  if (ret == false)
    cout << "Cannot get the session, will segfault" << endl;

  // There is nothing to read from
  if (session->content_in().size() == 0)
    {
      // Nothing to do
      session->services_->next_processing_stage(*session);
      return true;
    }

  // Read the content
  if (dataman::get_nextblock(session->hsock_con(), &content,
			     session->content_in().size(),
			     &nrecv, &err) == false)
    {
      reason = err;
      return false;
    }
  else
    {
      // Fill in the content buffer
      dataman::buffer buffer(content, session->content_in().size());
      session->content_in() = buffer;
      delete[] content;
      session->services_->next_processing_stage(*session);
    }

  return true;
}


bool send_response(sysapi::socket_in::handle_t& hsock,
		   dataman::buffer*,
		   sysapi::socket_in::error_t& sockerr)
{
  // ?
  // Send the whole http request on the wire

  sysapi::socket_in::size_t nrsent;
  dataman::buffer buf;
  http::session* session;
  bool ret;


  // Get the session address, gore mode
  ret = services_->find_session_byid(hsock, session);
  if (ret == false)
    cout << "Cannot get the session, will segfault" << endl;


  // Cooking lesson:
  // Build the response buffer
  // wrap it into an iovec
  // and call the write service

  buf += session->hdrlines_out();
  buf += session->content_out();

  { // Debugging purposes
    ostringstream strm("chunked(");
    if (session->first_chunk() == true) strm << "first)";
    else if (session->last_chunk() == true) strm << "last)";
    else strm << "middle)";
    strm << ", size(0x" << hex << (unsigned int)buf.size() << ")";
    cout << "\t[?] Sending response callback" << endl;
    cout << "\t\t[+] Sender identity    : " << std::dec << (unsigned int)session->hsock_con() << endl;
    cout << "\t\t[+] Response attributes: " << strm.str() << endl;
    if (session->hdrlines_out().size() != 0)
      {
	cout << "\t\t[+] Response header    : "
	     << "size(0x" << hex
	     << (unsigned int)session->hdrlines_out().size()
	     << ")" << endl;
	// cout << session->hdrlines_out().to_string(16) << endl;
      }
    else
      cout << "\t\t[+] Response header    : none" << endl;
    cout << "\t\t[+] Response body      : " << "size(0x"
	 << hex << (unsigned int)session->content_out().size()
	 << ")" << endl;
    // cout << session->content_out().to_string(16) << endl;
  }

  if (sysapi::socket_in::send(session->hsock_con(),
			      (unsigned char*)buf,
			      buf.size(),
			      &nrsent,
			      &sockerr) == false)
    {
      sysapi::error::stringify("[!] Sending has failed: ");
      return false;
    }

  if (nrsent != buf.size())
    {
      cout << ":Cannot Send the whole buffer, this is a simple buffer to fix" << endl;
      return false;
    }

  session->hdrlines_out().reset();
  session->content_out().reset();

  return true;
}


bool	close_connection(sysapi::socket_in::handle_t& hsock,
			 dataman::buffer*,
			 sysapi::socket_in::error_t&)
{
  // !
  // Here release the data used in inet_helper.cc
  cout << "\t\t[? " << (unsigned int)hsock << "]: Closing connection" << endl;
  return sysapi::socket_in::terminate_connection(hsock);
}
