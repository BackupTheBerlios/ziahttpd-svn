//
// compimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 09:58:21 2006 texane
// Last update Mon Apr  3 15:34:58 2006 
//


#include "include/mod_encoding.hh"


using namespace std;


#ifndef _WIN32
static int cicmp(char a, char b)
{
  // normalize
  if (a >= 'A' && a <= 'Z')
    a += 'a' - 'A';
  if (b >= 'A' && b <= 'Z')
    b += 'a' - 'A';

  if (a == b)
    return 0;
  return b - a;
}

int stricmp(const char* s1, const char* s2)
{
  while (cicmp(*s1, *s2) == 0)
    {
      if (*s1 == 0)
	return 0;
      ++s1;
      ++s2;
    }
  return *s2 - *s1;
}
#endif // !_WIN32


// implement ICompressor interface

void* mod_encoding::GetNewContext(const char* encoding)
{
  context_t* p_context;

  p_context = new context_t;
  p_context->encoding = encoding;
  p_context->u.context = 0;

  if (!stricmp(encoding, "deflate"))
    ZlibGetNewContext(p_context->u.zlib_context);
  else if (!stricmp(encoding, "gzip"))
    GzipGetNewContext(p_context->u.gzip_context);
  else if (!stricmp(encoding, "compress"))
    GzipGetNewContext(p_context->u.gzip_context);
  
  return p_context;
}

void mod_encoding::DestroyContext(void* p_context)
{
  if (((context_t*)p_context)->encoding == "deflate")
    ZlibDestroyContext(((context_t*)p_context)->u.zlib_context);
  else if (((context_t*)p_context)->encoding == "gzip")
    GzipDestroyContext(((context_t*)p_context)->u.gzip_context);
  else if (((context_t*)p_context)->encoding == "compress")
    GzipDestroyContext(((context_t*)p_context)->u.gzip_context);
  
  delete (context_t*)p_context;
}

bool mod_encoding::Compress(void* p_context, IBuffer& buf_in, IBuffer& buf_out)
{
  bool is_success;

  is_success = true;
  if (((context_t*)p_context)->encoding == "deflate")
    is_success = ZlibCompress(((context_t*)p_context)->u.zlib_context, buf_in, buf_out);
  else if (((context_t*)p_context)->encoding == "gzip")
    is_success = GzipCompress(((context_t*)p_context)->u.gzip_context, buf_in, buf_out);
  else if (((context_t*)p_context)->encoding == "compress")
    is_success = GzipCompress(((context_t*)p_context)->u.gzip_context, buf_in, buf_out);

  return is_success;
}

bool mod_encoding::Decompress(void* p_context, IBuffer& buf_in, IBuffer& buf_out)
{
  bool is_success;

  is_success = true;
  if (((context_t*)p_context)->encoding == "deflate")
    is_success = ZlibDecompress(((context_t*)p_context)->u.zlib_context, buf_in, buf_out);
  else if (((context_t*)p_context)->encoding == "gzip")
    is_success = GzipDecompress(((context_t*)p_context)->u.gzip_context, buf_in, buf_out);
  else if (((context_t*)p_context)->encoding == "deflate")
    is_success = GzipDecompress(((context_t*)p_context)->u.gzip_context, buf_in, buf_out);
  
  return is_success;
}


static const char* _encodings[] =
  {
    "gzip",
    "deflate",
    "compress",
    0
  };

const char** mod_encoding::GetSupportedEncoding()
{
  return _encodings;
}
