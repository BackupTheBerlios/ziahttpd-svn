//
// resource_bodydata.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Dec 04 16:24:04 2005 texane
// Last update Sun Dec 04 18:41:27 2005 texane
//


#include <iostream>
#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>
#include <dataman/inet_helper.hh>


using std::cerr;
using std::cout;
using std::endl;


dataman::bodydata::bodydata(sysapi::socket_in::handle_t hsock,
			    sysapi::socket_in::size_t szbody)
{
  hsock_ = hsock;
  szbody_ = szbody;
  szread_ = 0;
  opened_ = false;
}


dataman::bodydata::~bodydata()
{
}


bool	dataman::bodydata::open(openmode_t omode, error_t& err)
{
  bool ret = true;

  ret = true;

  // if it has already been opened
  if (opened_ == true)
    {
      err = ALREADYOPENED;
      return false;
    }
  opened_ = true;
  szread_ = 0;

  // this is a fetchonly resource
  if (omode != O_FETCHONLY)
    {
      err = OPNOTSUP;
      return false;
    }

  // no byte has been read yet
  szread_ = 0;

  return ret;
}


bool	dataman::bodydata::fetch(buffer& buf,
				 unsigned int nbuf,
				 error_t& err)
{
  bool ret;
  sysapi::socket_in::size_t nread;
  unsigned char* wrk;

  ret = true;
  err = ESUCCESS;

  if (opened_ == false)
    {
      ret = false;
      err = OPNOTSUP;
    }
  else
    {
      if (szread_ == szbody_)
	{
	  err = EOFETCHING;
	  ret = true;
	}
      else
	{
	  ret = dataman::get_nextblock(hsock_,
				       &wrk,
				       nbuf,
				       &nread);
	  if (ret == false)
	    err = OPFAILED;
	  else
	    {
	      buf = buffer(wrk, nread);
	      szread_ += nread;
	      delete wrk;
	    }
	}
    }

  return ret;
}


bool	dataman::bodydata::fetch(buffer& buf, error_t& err)
{
  return fetch(buf, szbody_, err);
}


bool	dataman::bodydata::feed(buffer&, error_t& err)
{
  err = OPNOTSUP;
  return false;
}


bool	dataman::bodydata::close(error_t&)
{
  opened_ = false;
  hsock_ = 0;
  szread_ = 0;
  szbody_ = 0;
  return false;
}
