//
// resource_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:53:14 2005 texane
// Last update Wed Nov 30 13:46:36 2005 texane
//


#include <string>
#include <iostream>
#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>


using std::cerr;
using std::cout;
using std::endl;
using std::string;
using dataman::buffer;
using dataman::resource::error_t;


dataman::file::file(const string& filename)
{
  // The file will be actually opened
  // when the call to the open method
  // is made.

  opened_ = false;
  filename_ = filename;
  nrtoread_ = 0;
}


dataman::file::~file()
{
  if (opened_ == true)
    sysapi::file::close(hfile_);
}


bool	dataman::file::open(openmode_t omode, error_t& err)
{
  bool ret;

  if (opened_ == true)
    {
      // The file has already been opened
      err = ALREADYOPENED;
      return false;
    }

  if (omode == O_FETCHONLY)
    {
      // Fetching only resource (cgi without post...)
    }
  else if (omode == O_FEEDONLY)
    {
      // Feeding only resource (put body in file...)
    }
  else
    {
      // Cannot open a file in both feed and fetch mode
      err = OPNOTSUP;
      return false;
    }

  omode_ = omode;

  if (sysapi::file::size(filename_.c_str(), &sz_) == false)
    {
      sysapi::error::stringify("Cannot open the file");
      return false;
    }

  ret = sysapi::file::open(&hfile_, filename_.c_str(), sysapi::file::RDONLY);
  if (ret == false)
    {
      std::cout << "open failed OIPENLKDSFJKLFJ" <<  std::endl;
      return false;
    }

  nrtoread_ = sz_;
  opened_ = true;

  return true;
}


bool	dataman::file::fetch(buffer& buf, unsigned int nrtoread, error_t& err)
{
  bool ret;
  unsigned char* wrk;
  unsigned int nread;

  err = ESUCCESS;

  if (opened_ == false)
    {
      err = NOTOPENED;
      return false;
    }

  if (omode_ == O_FEEDONLY)
    {
      err = OPNOTSUP;
      return false;
    }

  if (nrtoread_ == 0)
    {
      err =  EOFETCHING;
      return true;
    }
  
  // Normalize the buffer size to chunk's one
  if (nrtoread > nrtoread_)
    nrtoread = nrtoread_;

  wrk = new unsigned char[nrtoread];
  ret = sysapi::file::read(hfile_,
			   wrk,
			   nrtoread,
			   reinterpret_cast<sysapi::file::size_t*>(&nread));

  if (ret == true)
    {
      buf = buffer(wrk, nread);
      nrtoread_ -= nread;
    }

  if (nrtoread_ ==0)
    err = EOFETCHING;

  delete[] wrk;

  return true;
}


bool	dataman::file::fetch(buffer& buf, error_t& err)
{
  if (opened_ == false)
    {
      err = NOTOPENED;
      return false;
    }

  return fetch(buf, sz_, err);
}


bool	dataman::file::feed(buffer&, error_t&)
{
  cerr << "feeding methood not implemented fro the filereource" << endl;
  return true;
}


bool	dataman::file::close(error_t& err)
{
  if (opened_ == false)
    {
      err = NOTOPENED;
      return false;
    }

  sysapi::file::close(hfile_);
  opened_ = false;

  return true;
}
