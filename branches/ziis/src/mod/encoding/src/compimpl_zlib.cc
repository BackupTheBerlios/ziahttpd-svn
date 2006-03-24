//
// complimpl_zlib.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 10:24:08 2006 texane
// Last update Fri Mar 24 02:45:55 2006 texane
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
  int ret;

  cout << "entering compress" << endl;

  sz_out = sz_in = buf_in.Length();
  if (sz_in == 0)
    {
      buf_out.Clear();
      return false;
    }

  p_in = (unsigned char*)buf_in.Str();
  p_out = new unsigned char[sz_out];

  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
  if (ret != Z_OK)
    {
      cout << "error: deflateInit" << endl;
      return false;
    }

  strm.avail_in = sz_in;
  strm.next_in = p_in;
  strm.avail_out = sz_out;
  strm.next_out = p_out;

  ret = deflate(&strm, Z_FINISH);
  if (ret == Z_OK)
    cout << "deflate returned ok" << endl;
  else if (ret == Z_STREAM_END)
    cout << "deflate returned end" << endl;
  else if (ret == Z_BUF_ERROR)
    cout << "deflate returned BUF_ERROR" << endl;
  else
    cout << "error: deflate" << endl;
  deflateEnd(&strm);

  cout << "reset done" << endl;

  buf_out.Clear();
  buf_out.Append((const char*)p_out, sz_out - strm.avail_out);
  delete[] p_out;

  cout << "exiting compress: " << sz_out - strm.avail_out << endl;

  return true;
}
