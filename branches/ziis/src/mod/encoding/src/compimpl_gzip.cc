//
// compimpl_gzip.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Mar 24 12:12:20 2006 texane
// Last update Fri Mar 24 12:13:24 2006 texane
//


#include "include/mod_encoding.hh"


using namespace std;


bool mod_encoding::GzipGetNewContext(gzip_context_t*& p_context)
{
  p_context = 0;
  return true;
}

bool mod_encoding::GzipDestroyContext(gzip_context_t* p_context)
{
  if (p_context)
    {
      delete p_context;
    }
  return true;
}

bool mod_encoding::GzipDecompress(gzip_context_t*, IBuffer& buf_in, IBuffer& buf_out)
{
  buf_out = buf_in;
  return true;
}

bool mod_encoding::GzipCompress(gzip_context_t* p_context, IBuffer& buf_in, IBuffer& buf_out)
{
  buf_out = buf_in;
  return true;
}

