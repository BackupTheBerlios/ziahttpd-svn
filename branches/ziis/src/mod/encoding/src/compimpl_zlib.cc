//
// complimpl_zlib.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 10:24:08 2006 texane
// Last update Fri Mar 24 13:06:00 2006 texane
//


#include "include/mod_encoding.hh"


using namespace std;


bool mod_encoding::ZlibGetNewContext(zlib_context_t*& p_context)
{
  p_context = 0;
  return true;
}

bool mod_encoding::ZlibDestroyContext(zlib_context_t* p_context)
{
  if (p_context)
    {
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
  unsigned char* p_out;
  unsigned char* p_in;
  unsigned int sz_in;
  unsigned int sz_out;
  z_stream strm;
  int st_deflate;
  int nr_ret;
  bool is_done;

  cout << "entering ZLIB: " << buf_in.Length() << endl;

  buf_out.Clear();
  sz_out = sz_in = buf_in.Length();
  if (sz_in == 0)
    goto end_of_compress;

  p_in = (unsigned char*)buf_in.Str();
  p_out = new unsigned char[sz_out];

  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  nr_ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
  if (nr_ret != Z_OK)
    return false;

  strm.avail_in = sz_in;
  strm.next_in = p_in;

  is_done = false;
  st_deflate = Z_FINISH;
  while (is_done == false)
    {
      strm.avail_out = sz_out;
      strm.next_out = p_out;

      nr_ret = deflate(&strm, st_deflate);
      if (nr_ret == Z_STREAM_END)
	{
	  is_done = true;
	}
      else if (nr_ret != Z_OK)
	{
	  cout << "error: ZLIB" << endl;
	  is_done = true;
	}
      buf_out.Append((const char*)p_out, sz_out - strm.avail_out);

    }
  deflateEnd(&strm);
  delete[] p_out;

  cout << "exiting ZLIB: " << buf_out.Length() << endl;

 end_of_compress:
  return true;
}
