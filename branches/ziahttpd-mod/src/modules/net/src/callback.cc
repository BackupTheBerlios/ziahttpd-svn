//
// callback.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Nov 22 19:44:26 2005 texane
// Last update Wed Nov 23 12:02:48 2005 texane
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


bool read_httpheaders(http::session* session, server::service::iovec_t& iov)
{
  sysapi::socket_in::error_t err;
  char* line;

  // Get the status line
  {
    if (dataman::get_nextline(session->hsock_con(), &line, &err) == false)
      {
	return false;
      }

    dataman::buffer buffer(reinterpret_cast<const unsigned char*>(line), strlen(line));
    session->hdrlines_in().push_front(buffer);
    free(line);
  }

  // Read headerlines
  while (dataman::get_nextline(session->hsock_con(), &line, &err) && strlen(line))
    {
      dataman::buffer buffer(reinterpret_cast<unsigned char*>(line), strlen(line));
      session->hdrlines_in().push_back(buffer);
      free(line);
    }

  // Request the server to register new callback
  session->services_->register_callback(*session, server::service::EVREAD, read_httpbody);
  // Perform the io
  session->services_->perform_io(*session, server::service::EVREAD);

  return true;
}


bool read_httpbody(http::session* session, server::service::iovec_t& iov)
{
  unsigned char* content;
  sysapi::socket_in::size_t ncontent;

  // There is nothing to read from
  if (session->content_in().size() == 0)
    return true;

  // Read the content
  content = new unsigned char[session->content_in().size()];
  if (sysapi::socket_in::recv(session->hsock_con(), content, session->content_in().size(), &ncontent) == false)
    {
      return false;
    }

  // Fill in the content buffer
  {
    dataman::buffer buffer(content, ncontent);
    session->content_in() = buffer;
    delete[] content;
  }

  return true;
}


bool read_cgistdout(http::session*, server::service::iovec_t&)
{
  return false;
}


bool write_httpresponse(http::session* session, server::service::iovec_t&)
{
  // ?
  // Send the whole http request on the wire

  dataman::buffer buf;

  // Cooking lesson:
  // Build the response buffer
  // wrap it into an iovec
  // and call the write service
  buf += session->hdrlines_out();
  buf += session->content_out();

  cout << "sending over " << buf.size() << " bytes" << endl;

  if (sysapi::socket_in::send(session->hsock_con(), (unsigned char*)buf, buf.size()) == false)
    {
      return false;
    }

  return true;
}


bool write_cgistdin(http::session*, server::service::iovec_t&)
{
  return false;
}


bool close_httpconnection(http::session* session, server::service::iovec_t&)
{
  return sysapi::socket_in::terminate_connection(session->hsock_con());
}
