//
// resource_cgi.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:53:20 2005 texane
// Last update Fri Dec 02 14:05:29 2005 texane
//


#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>


using std::cout;
using std::endl;
using std::vector;
using std::string;
using dataman::buffer;


dataman::cgi::cgi(const vector<const string>& av,
		  const vector<const string>& env)
{
  reset();

  // Copy av
  av_ = new char*[av.size() + 1];
  av_[av.size()] = 0;
  ac_ = av.size();
  for (unsigned int i = 0; i < av.size(); ++i)
    {
      av_[i] = new char[av[i].size() + 1];
      strcpy(av_[i], av[i].c_str());
    }

  // Copy environ block
  env_ = new char*[env.size() + 1];
  env_[env.size()] = 0;
  for (unsigned int i = 0; i < env.size(); ++i)
    {
      env_[i] = new char[env[i].size() + 1];
      strcpy(env_[i], env[i].c_str());
    }
}


dataman::cgi::~cgi()
{
  release();
}


bool	dataman::cgi::open(openmode_t omode, error_t& err)
{
  bool ret;

  if (allocated_ == true)
    {
      err = ALREADYOPENED;
      return false;
    }

  if (omode == O_FEEDONLY)
    {
      err = OPNOTSUP;
      return false;
    }
  else if (omode == O_FETCHONLY)
    {
      // The process is to be output only redirected
      feeding_ = true;
      ret = sysapi::process::create_outredir_and_loadexec(&hproc_,
							  &hout_,
							  ac_,
							  const_cast<const char**>(av_),
							  const_cast<const char**>(env_));
    }
  else
    {
      // The process is to be in/out redirected
      ret = sysapi::process::create_inoutredir_and_loadexec(&hproc_,
							    &hout_,
							    &hin_,
							    ac_,
							    const_cast<const char**>(av_),
							    const_cast<const char**>(env_));
    }

  if (ret == true)
    {
      allocated_ = true;
      // delete resource from process (?)
    }

  omode_ = omode;

  return ret;
}


bool	dataman::cgi::fetch(buffer& buf, unsigned int nbytes, error_t& err)
{
  bool ret;
  sysapi::file::size_t nread;
  sysapi::process::state_t st;
  unsigned char* wrk;
  
  err = ESUCCESS;

  // Thus, fetching is true
  feeding_ = false;

  wrk = new unsigned char[nbytes];
  cout << "reading..." << endl;
  ret = sysapi::file::read(hout_,
			   wrk,
			   nbytes,
			   &nread);
  if (ret == false)
    {
      sysapi::error::stringify("Cannot read");
      cout << "READ FAILED" << endl;
      delete wrk;
      err = OPFAILED;
      return false;
    }

  // Set the size we have just read
  buf = buffer(wrk, nread);
  delete wrk;

  // Is the process done, dont' block if not
//   ret = sysapi::process::wait_single(hproc_, &st, sysapi::process::DONTWAIT);

  // (!) Here should check the status
  // code of the process

  // The process is not done, not an error
//   if (ret == false)
//     return true;

  // Here the proces sis now done,
  // so release associated resources
//   err = EOFETCHING;

//   release();
//   reset();

  return true;
}


bool	dataman::cgi::fetch(buffer&, error_t& err)
{
  // We havn't the size of cgi process
  // produced output, so this request
  // is invalid.

  err = OPNOTSUP;
  return false;
}


bool	dataman::cgi::feed(buffer& buf, error_t& err)
{
  bool ret;
  sysapi::file::size_t nrfed;

  if (allocated_ == false)
    {
      err = NOTOPENED;
      return false;
    }

  if (omode_ == O_FETCHONLY || feeding_ == false)
    {
      err = OPNOTSUP;
      return false;
    }

  // Actually feed the buffer
  ret = sysapi::file::write(hin_,
			    (unsigned char*)buf,
			    buf.size(),
			    &nrfed);
  if (ret == false)
    {
      err = OPFAILED;
      return false;
    }

  return true;
}


bool	dataman::cgi::close(error_t& err)
{
  // Release memory allocated resources

  if (allocated_ == false)
    {
      err = NOTOPENED;
      return false;
    }

  release();
  reset();

  return true;
}


void	dataman::cgi::reset()
{
  // Reset the cgi related data.
  // By default, input is used,
  // so the process is to be created
  // in and output redirected.

  // Default execution environment
  av_ = 0;
  env_ = 0;
  ac_ = 0;
  
  // Default creation flag
  allocated_ = false;
  feeding_ = true;
  hin_inuse_ = false;  
}


bool	dataman::cgi::release()
{
  if (allocated_ == false)
    return false;

  if (av_)
    {
      for (int i = 0; av_[i]; ++i)
	delete av_[i];
      delete[] av_;
      ac_ = 0;
    }

  if (env_)
    {
      for (int i = 0; env_[i]; ++i)
	delete env_[i];
      delete[] env_;
    }

  if (hin_inuse_ == true)
    sysapi::file::close(hin_);
  sysapi::file::close(hout_);
  sysapi::process::release(hproc_);

  reset();
}
