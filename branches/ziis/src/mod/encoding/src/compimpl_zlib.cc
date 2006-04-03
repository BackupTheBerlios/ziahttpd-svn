//
// complimpl_zlib.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 10:24:08 2006 texane
// Last update Mon Apr 03 20:04:04 2006 texane
//


#include "include/mod_encoding.hh"


using namespace std;


bool mod_encoding::ZlibGetNewContext(zlib_context_t*& p_context)
{
  p_context = new zlib_context_t;
  p_context->strm_encoding.zalloc = Z_NULL;
  p_context->strm_encoding.zfree = Z_NULL;
  p_context->strm_encoding.opaque = Z_NULL;
  
  p_context->nr_return = deflateInit(&p_context->strm_encoding, Z_DEFAULT_COMPRESSION);
  p_context->sz_out = 0;
  p_context->p_out = 0;
  p_context->p_in = 0;
  p_context->sz_in = 0;
//   p_context->nr_flush = Z_NO_FLUSH;
  p_context->nr_flush = Z_FINISH;
  p_context->is_done = false;

  return true;
}

bool mod_encoding::ZlibDestroyContext(zlib_context_t* p_context)
{
  if (p_context)
    {
      if (p_context->p_out)
	delete[] p_context->p_out;
      deflateEnd(&p_context->strm_encoding);
      delete p_context;
    }
  return true;
}

bool mod_encoding::ZlibDecompress(zlib_context_t*, IBuffer& buf_in, IBuffer& buf_out)
{
  buf_out = buf_in;
  return true;
}

bool mod_encoding::ZlibCompress(zlib_context_t* p_context, IBuffer& buf_in, IBuffer& buf_out)
{
  unsigned int nr_avail;
  unsigned char* p_next;
  bool is_done;

  // return false on error
  if (p_context->nr_return != Z_OK)
    return false;

  // reset
  buf_out.Clear();

  // first init
  if (p_context->p_out == 0)
    {
      p_context->sz_in = buf_in.Length();
      p_context->sz_out = buf_in.Length();
      p_context->p_out = new unsigned char[buf_in.Length()];
      p_context->p_in = (unsigned char*)buf_in.Str();
      p_context->strm_encoding.next_in = (unsigned char*)buf_in.Str();
      p_context->strm_encoding.avail_in = buf_in.Length();
    }

  is_done = false;
  p_next = p_context->p_out;
  nr_avail = p_context->sz_out;
  while (is_done == false)
    {
      p_context->strm_encoding.next_out = p_next;
      p_context->strm_encoding.avail_out = nr_avail;
      p_context->nr_return = deflate(&p_context->strm_encoding, p_context->nr_flush);
      if (p_context->nr_return == Z_STREAM_END)
	{
	  is_done = true;
	}
      else if (p_context->nr_return != Z_OK)
	{
	  is_done = true;
	}
      buf_out.Append((const char*)p_next, nr_avail - p_context->strm_encoding.avail_out);
      nr_avail = p_context->strm_encoding.avail_out;
    }
  return true;
}
