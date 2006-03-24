//
// mod_encoding.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 09:54:28 2006 texane
// Last update Fri Mar 24 12:33:05 2006 texane
//


#ifndef MOD_ENCODING_HH
# define MOD_ENCODING_HH


#include <iostream>
#include <string>
#include <string.h>
#include <sys/sysapi.hh>
#include <ziis.hh>
#include "../lib/zlib-1.2.3/zlib.h"
// #include "../lib/gzip124/gzip.h"


class mod_encoding : public IModule, public ICompressor
{
public:
  // ctor/dtor
  mod_encoding();
  ~mod_encoding();

  // implement IModule
  bool ReadConfig(const char*);
  const char* GetModuleName();
  const char* GetModuleVersion();
  void OnLoad(IFS*);

  // implement ICompressor
  void* GetNewContext(const char*);
  void DestroyContext(void*);
  bool Compress(void*, IBuffer&, IBuffer&);
  bool Decompress(void*, IBuffer&, IBuffer&);
  const char** GetSupportedEncoding();

private:
  // Zlib compression
  typedef struct
  {
    z_stream strm_encoding;
  } zlib_context_t;
  bool ZlibGetNewContext(zlib_context_t*&);
  bool ZlibDestroyContext(zlib_context_t*);
  bool ZlibDecompress(zlib_context_t*, IBuffer&, IBuffer&);
  bool ZlibCompress(zlib_context_t*, IBuffer&, IBuffer&);

  // Gzip compression
  typedef struct
  {
    unsigned int fake;
  } gzip_context_t;
  bool GzipGetNewContext(gzip_context_t*&);
  bool GzipDestroyContext(gzip_context_t*);
  bool GzipDecompress(gzip_context_t*, IBuffer&, IBuffer&);
  bool GzipCompress(gzip_context_t*, IBuffer&, IBuffer&);


  // general context
  typedef struct
  {
    std::string encoding;
    union
    {
      zlib_context_t* zlib_context;
      gzip_context_t* gzip_context;
      void* context;
    } u;
  } context_t;
};


#endif // ! MOD_ENCODING_HH
