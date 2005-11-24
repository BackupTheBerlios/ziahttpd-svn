//
// resource_report.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:53:31 2005 texane
// Last update Thu Nov 24 21:27:34 2005 texane
//


#include <string>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>


using std::string;
using dataman::buffer;


dataman::report::report(unsigned int stcode)
{
  stcode_ = stcode;
  formed_ = false;
}


dataman::report::~report()
{}


bool	dataman::report::open(openmode_t omode, error_t& err)
{
  // !
  // Generate a more elaborated function

  if (omode != O_FETCHONLY)
    {
      // This kind of resource cannot be
      // opened in a wriable mode.
      err = OPNOTSUP;
      return false;
    }

  if (formed_ == true)
    {
      err = ALREADYOPENED;
      return false;
    }

  switch (stcode_)
    {
    case 404:
      buf_ = "<html><body><h1>404 Not Found</h1></body></html>";
      break;
    default:
      buf_ = "<html><body><h1>Default Report Page</h1></body></html>";
      break;
    }

  formed_ = true;
  return true;
}


bool	dataman::report::fetch(buffer& buf, unsigned int nbytes, error_t&)
{
  string substr;

  // Normalize the size
  if (buf_.size() < nbytes)
    nbytes = buf_.size();

  // Take the substring and remove
  // nbytes from the buffer
  substr.append(buf_, 0, nbytes);
  buf_.erase(0, nbytes);
  buf = substr;

  return true;
}


bool	dataman::report::fetch(buffer& buf, error_t&)
{
  buf = buf_;
  return true;
}


bool	dataman::report::feed(buffer&, error_t& err)
{
  // This kind of resource cannot
  // be fed; This is an in memory
  // buffer, generated only by the
  // core server.

  err = OPNOTSUP;
  return false;
}


bool	dataman::report::close(error_t&)
{
  // release the allocated memory
  // for the buffer (for the moment,
  // there is no memory, store in .data)
  return true;
}
