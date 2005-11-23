//
// resource_file.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:53:14 2005 texane
// Last update Wed Nov 23 22:02:21 2005 texane
//


#include <string>
#include <iostream>
#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>


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
}


bool	dataman::file::open(error_t& err)
{
  bool ret;

  if (opened_ == true)
    {
      err = ALREADYOPENED;
      return false;
    }

  if (sysapi::file::size("..\\root\\www\\index.html", &sz_) == false)
    {
      sysapi::error::stringify("Cannot open the file");
      return false;
    }

  ret = sysapi::file::open(&hfile_, "..\\root\\www\\index.html", sysapi::file::RDONLY);
  //   ret = sysapi::file::open(&hfile_, filename_.c_str(), sysapi::file::RDONLY);
  if (ret == false)
    {
      std::cout << "OIPENLKDSFJKLFJ" <<  std::endl;
      return false;
    }

  opened_ = true;

  return true;
}


bool	dataman::file::fetch(buffer& buf, unsigned int nbytes, error_t& err)
{
  bool ret;
  unsigned char* wrk;
  unsigned int nread;

  if (opened_ == false)
    {
      err = NOTOPENED;
      return false;
    }

  wrk = new unsigned char[nbytes];
  ret = sysapi::file::read(hfile_, wrk, nbytes, reinterpret_cast<sysapi::file::size_t*>(&nread));

  if (ret == true)
    buf = buffer(wrk, nread);

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
