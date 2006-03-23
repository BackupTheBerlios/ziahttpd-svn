//
// mod_encoding.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Mar 23 09:54:28 2006 texane
// Last update Thu Mar 23 09:55:08 2006 texane
//


#ifndef MOD_ENCODING_HH
# define MOD_ENCODING_HH


#include <iostream>
#include <string>
#include <sys/sysapi.hh>
#include <ziis.hh>


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
};


#endif // ! MOD_ENCODING_HH
