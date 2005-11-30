//
// callback.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Nov 22 19:44:26 2005 texane
// Last update Wed Nov 30 14:12:00 2005 texane
//


#include <string>
#include <cstring>
#include <iostream>
#include <zia.hh>
#include <callback.hh>


using std::string;


// This file export callbacks to be
// called by server on events.
// The request processing has different
// stages, so we define several callback
// for {in, out} put operations.


extern server::service* services_;


bool read_metadata(sysapi::socket_in::handle_t& hsock,
		   dataman::buffer*,
		   sysapi::socket_in::error_t&)
{
  bool ret;
  char* line;
  http::session* session;
  sysapi::socket_in::error_t err;

  // Get the session address
  ret = services_->find_session_byid(hsock, session);
  if (ret == false)
    cout << "Cannot get the session, will segfault" << endl;

  //
  cout << "\t\t[+]Reading metadata" << endl;

  // Read the next http line
  while ((ret = dataman::get_nextline(hsock, &line, &err)) == true)
    {
      if (strlen(line) != 0)
	{
	  // It is a header line
	  dataman::buffer buffer(reinterpret_cast<const unsigned char*>(line), strlen(line));
	  session->hdrlines_in().push_back(buffer);
	  cout << "\t\t[+] new header line: " << buffer.c_str() << endl;
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

  // It is not the header end
  return false;
}


bool read_data(sysapi::socket_in::handle_t& hsock,
	       dataman::buffer*,
	       sysapi::socket_in::error_t&)
{
  unsigned char* content;
  sysapi::socket_in::size_t ncontent;
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
      cout << "\t\t[+]Getnextblock, size: " << session->content_in().size() << ", got " << nrecv << endl;
      return false;
    }
  else
    {
      // Fill in the content buffer
      dataman::buffer buffer(content, ncontent);
      session->content_in() = buffer;
      delete[] content;
      session->services_->next_processing_stage(*session);
    }

  return true;
}


bool send_response(sysapi::socket_in::handle_t& hsock,
		   dataman::buffer*,
		   sysapi::socket_in::error_t&)
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

//   cout << "\t\t[+] Sending response" << endl;
//   cout << buf.to_string(16) << endl;

  if (sysapi::socket_in::send(session->hsock_con(), (unsigned char*)buf, buf.size(), &nrsent) == false)
    {
      cout << ":Sending has failed!" << endl;
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
  return sysapi::socket_in::terminate_connection(hsock);
}
