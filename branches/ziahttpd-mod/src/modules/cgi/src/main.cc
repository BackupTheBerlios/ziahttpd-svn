//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Mon Nov 21 20:04:53 2005 texane
//


#include <zia.hh>
#include <string>
#include <iostream>
#include <sstream>


using std::cout;
using std::cerr;
using std::endl;
using std::string;
using dataman::buffer;
using http::session;


// List of exported functions
MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session&, server::core*, int&);


// Exported function definitions


static bool fetch_content_from_cgi(
				   buffer& bufsrc,
				   buffer& bufdst,
				   int ac,
				   const char** av,
				   const char** env
				   )
{
  // !
  // This function should be recoded in
  // order to use http chunked body feature.
  // It should use the service io services in
  // order for our callback to be called when
  // a next block is available from the cgi output.
  // Idem for fetching the buffer.

  bool ret;
  bool hin_inuse;
  sysapi::file::handle_t hout;
  sysapi::file::handle_t hin;
  sysapi::process::handle_t hproc;

  hin_inuse = false;

  // Create redirected process for the cgi execution
  if (bufsrc.size() == 0)
    {
      // In our case, this is a get method,
      // only the out endpoint has to be redir'ed
      ret = sysapi::process::create_outredir_and_loadexec(&hproc,
							  &hout,
							  ac, av, env);
    }
  else
    {
      // In our case, this is a post request,
      // the two pipe endpoints have to be redir'ed.
      ret = sysapi::process::create_inoutredir_and_loadexec(&hproc,
							    &hout, &hin,
							    ac, av, env);
      hin_inuse = true;
    }

  if (ret == false)
    {
      sysapi::error::stringify("Cannot create process for the cgi");
      return false;
    }

  // Fetch the process by writing to
  // the hin handle
  if (bufsrc.size() && hin_inuse == true)
    {
      unsigned char* ptrbuf = (unsigned char*)bufsrc;
      if (sysapi::file::write(hout, const_cast<const unsigned char*>(ptrbuf), bufsrc.size()) == false)
	{
	  sysapi::error::stringify("Cannot write src buffer to cgi");
	  ret = false;
	}
    }

  // Read from the process(hout),
  // and wait for it to end
  {
    // ! Do more error checks
    buffer tmpbuf(hout);
    sysapi::process::wait_single(hproc);
    sysapi::process::release(hproc);
    bufdst = tmpbuf;
  }

  // Close file handles
  sysapi::file::close(hout);
  if (hin_inuse == true)
    sysapi::file::close(hin);

  return ret;
}


static bool prepare_process_context(session& session,
				    int& ac,
				    char**& av,
				    char**& env)
{
  // ?
  // Prepare the process context
  // according to http informamtions stored
  // in the current session.
  // This buffer has to be deleted by the caller.

  ac = 1;
  av = new char*[ac + 1];
  av[0] = const_cast<char*>(session.uri().localname().c_str());
  av[1] = 0;
  env = 0;

  cout << "av[0] is " << av[0] << endl;

  return true;
}


static bool post_process_context(char** av,
				 char** env)
{
  // Release the process context.

  if (av)
    {
      for (int i = 0; av[i]; ++i)
	delete[] av[i];
      delete[] av;
    }

  if (env)
    {
      for (int i = 0; env[i]; ++i)
	delete[] env[i];
      delete[] env;
    }

  return true;
}


MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session& session, server::core*, int&)
{
  bool ret;

  ret = true;

  if (session.info_in().method_string() == "get")
    {
      int ac;
      char** av = 0;
      char** env = 0;

      cout << "\t[ * ] CGI MODULE CALLED with method <GET>" << endl;
      cout << "uri      : " << session.uri().widename() << endl;
      cout << "localname: " << session.uri().localname() << endl;
      cout << "extension: " << session.uri().extension() << endl;
      if (
	  prepare_process_context(session, ac, av, env) == true &&
	  fetch_content_from_cgi(session.content_in(),
				 session.content_out(),
				 ac,
				 const_cast<const char**>(av),
				 const_cast<const char**>(env))
	  )
	{
	  cout << "\t[ * ] Body susccessfuly got from cgi" << endl;
	}
      else
	{
	  ret = false;
	  cerr << "\t[ * ] Resource cgi failed to be loaded" << endl;
	}

      // Release the child process context
      post_process_context(av, env);

      // Alter the content length for the reponse
      // Remove from the hook and put it in the net one
      // + Add an exception for the process creation failure
      {
// 	std::ostringstream strm;
// 	strm << (unsigned int)session.content_out().size();
// 	cout << "size of the response body will be " << strm.str() << endl;
// 	(session.info_out())["content-length"] = strm.str();
      }

    }
  else
    {
      cout << "\t[ * ] CGI MODULE CALLED denying the request to be processed" << endl;
    }

  return ret;
}
