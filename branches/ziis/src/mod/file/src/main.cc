//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 11:02:05 2006 texane
// Last update Tue Mar 21 23:44:34 2006 texane
//


#include <sys/sysapi.hh>
#include <ziis.hh>


class mod_file : public IModule, public IDocumentGenerator
{
public:
  mod_file();
  ~mod_file();

  // implement IModule
  bool ReadConfig(const char*);
  const char* GetModuleName();
  const char* GetModuleVersion();
  void OnLoad(IFS*);

  // implement IDocumentGenerator
  void GenerateDocument(IInput&, const char*, IOutput&);
  const char** GetSupportedMime();
};


// module management

IModule* GetNewInstance()
{
  return new mod_file;
}

mod_file::mod_file()
{
}

mod_file::~mod_file()
{
}


// implement IModule

bool mod_file::ReadConfig(const char*)
{
  return true;
}

const char* mod_file::GetModuleName()
{
  return "mod_file";
}

const char* mod_file::GetModuleVersion()
{
  return "1.0";
}

void mod_file::OnLoad(IFS*)
{
}


// implement IDocumentGenerator
void mod_file::GenerateDocument(IInput&, const char*, IOutput&)
{
  
}

static const char* _my_mimes[] =
  {
    "text/html",
    0
  };

const char** mod_file::GetSupportedMime()
{
  return _my_mimes;
}
