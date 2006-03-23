//
// complimpl_zlib.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 10:24:08 2006 texane
// Last update Thu Mar 23 16:45:02 2006 texane
//


#include "include/mod_encoding.hh"


using namespace std;


bool mod_encoding::ZlibGetNewContext(zlib_context_t*& p_context)
{
  int err_code;
  bool is_success;
  bool is_deflat_inited;
  bool is_inflat_inited;

  // reset
  err_code = 0;
  is_success = true;
  is_deflat_inited = false;
  is_inflat_inited = false;
  p_context = new zlib_context_t;
  
//   // init inflate stream
//   p_context->strm_inflate.next_in = Z_NULL;
//   p_context->strm_inflate.avail_in = Z_NULL;
//   p_context->strm_inflate.zalloc = Z_NULL;
//   p_context->strm_inflate.zfree = Z_NULL;
//   p_context->strm_inflate.opaque = Z_NULL;
//   err_code = inflateInit(&p_context->strm_inflate);
//   if (err_code != Z_OK)
//     {
//       is_success = false;
//       goto zlib_init_error;
//     }
//   is_inflat_inited = true;

//   // init deflate stream
//   p_context->strm_deflate.zalloc = Z_NULL;
//   p_context->strm_deflate.zfree = Z_NULL;
//   p_context->strm_deflate.opaque = Z_NULL;
//   err_code = deflateInit(&p_context->strm_inflate, Z_DEFAULT_COMPRESSION);
//   if (err_code != Z_OK)
//     {
//       is_success = false;
//       goto zlib_init_error;
//     }
//   is_deflat_inited = true;

//   // error handling
//  zlib_init_error:
//   if (is_success == false)
//     {
//       if (is_inflat_inited == true)
// 	inflateEnd(&p_context->strm_inflate);
//       if (is_deflat_inited == true)
// 	deflateEnd(&p_context->strm_deflate);
//       delete p_context;
//       p_context = 0;
//     }

  return is_success;
}

bool mod_encoding::ZlibDestroyContext(zlib_context_t* p_context)
{
  delete p_context;
  return true;
}

bool mod_encoding::ZlibDecompress(zlib_context_t*, IBuffer& buf_in, IBuffer& buf_out)
{
  buf_out = buf_in;
  return true;
}

bool mod_encoding::ZlibCompress(zlib_context_t*, IBuffer& buf_in, IBuffer& buf_out)
{
  cout << "ZLIB COMPRESS" << endl;
  buf_out = buf_in;
  return true;
}
