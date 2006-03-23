//
// compimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 09:58:21 2006 texane
// Last update Thu Mar 23 11:52:11 2006 texane
//


#include "include/mod_encoding.hh"


using namespace std;


// implement ICompressor interface

void* mod_encoding::GetNewContext(const char* encoding)
{
  context_t* p_context;

  p_context = new context_t;
  p_context->encoding = encoding;
  p_context->u.context = 0;

  if (!strcmp(encoding, "deflate"))
    ZlibGetNewContext(p_context->u.zlib_context);
  
  return p_context;
}

void mod_encoding::DestroyContext(void* p_context)
{
  if (((context_t*)p_context)->encoding == "deflate")
    ZlibDestroyContext(((context_t*)p_context)->u.zlib_context);
  
  delete p_context;
}

bool mod_encoding::Compress(void* p_context, IBuffer& buf_in, IBuffer& buf_out)
{
  bool is_success;

  is_success = false;
  if (((context_t*)p_context)->encoding == "deflate")
    is_success = ZlibCompress(((context_t*)p_context)->u.zlib_context, buf_in, buf_out);
  return is_success;
}

bool mod_encoding::Decompress(void* p_context, IBuffer& buf_in, IBuffer& buf_out)
{
  bool is_success;

  is_success = false;
  if (((context_t*)p_context)->encoding == "deflate")
    is_success = ZlibDecompress(((context_t*)p_context)->u.zlib_context, buf_in, buf_out);
  return is_success;
}


static const char* _encodings[] =
  {
    "identity",
    "gzip",
    "deflate",
    "compress",
    0
  };

const char** mod_encoding::GetSupportedEncoding()
{
  return _encodings;
}
