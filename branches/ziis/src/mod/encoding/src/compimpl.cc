//
// compimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 09:58:21 2006 texane
// Last update Thu Mar 23 10:08:45 2006 texane
//


#include "include/mod_encoding.hh"


using namespace std;


// context related to encoding lib

typedef struct
{
  string encoding;
  union
  {
    void* data;
  } u;
} _context_t;


// implement ICompressor interface

void* mod_encoding::GetNewContext(const char* encoding)
{
  _context_t* p_context;

  p_context = new _context_t;
  p_context->encoding = encoding;
  p_context->u.data = 0;
  return p_context;
}

void mod_encoding::DestroyContext(void* p_context)
{
  delete p_context;
}

bool mod_encoding::Compress(void* p_context, IBuffer& buf_in, IBuffer& buf_out)
{
  return false;
}

bool mod_encoding::Decompress(void* p_conext, IBuffer& buf_in, IBuffer& buf_out)
{
  return false;
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
