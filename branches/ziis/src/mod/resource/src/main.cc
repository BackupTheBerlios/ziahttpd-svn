//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 11:02:05 2006 texane
// Last update Wed Mar 22 10:12:05 2006 texane
//


#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/sysapi.hh>
#include <ziis.hh>


using namespace std;


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
void mod_file::GenerateDocument(IInput& in, const char* path, IOutput& out)
{
//   bool is_done;
//   int nr_pass;
//   int nr_recv;
//   int nr_insz;
//   char buf[512];
//   const char* p_method;
//   const char* p_content_length;
//   ostringstream oss;

//   cout << "generating document" << endl;

//   // get the method
//   p_method = in.GetInputMethod();
//   if (method == 0)
//     return ;

//   // get the input size
//   nr_insz = 0;
//   p_content_length = in.GetInput("content-length");
//   if (p_content_length)
//     {
//       istringstream iss(p_content_length);
//       iss >> nr_insz;
//     }

//   // generate the resource
//   is_done = false;
//   nr_pass = 0;
//   while (is_done == false)
//     {
//       // something to recv from
//       if (nr_insz > 0)
// 	{
// 	  nr_recv = in.ReadPostEntity(buf, sizeof(buf));
// 	  if (nr_recv > 0)
// 	    nr_insz -= nr_recv;
// 	}

//       // feed the resource with buf

//       // generate content from resource

//       // generate header
//       if (nr_pass == 0)
// 	{
// 	  out.SendHeader();
// 	}
//       // generate chunk
//       else
// 	{

// 	}
//       ++nr_pass;

//       // send the buffer
//     }


//   if (strcmp(method, "GET") == 0)
//     {
//       oss << "<html><body>404 pas found</body></html>" << endl;
//       out.SendHeader();
//       out.SendBuffer(oss.str().c_str(), (int)strlen(oss.str().c_str()));
//     }
//   else if (strcmp(method, "POST") == 0)
//     {
//       cout << "post method not handled" << endl;
//     }
//   return ;



//   is_done = false;
//   nr_recv = 0;
//   iss >> nr_size;
//   cout << "generate document: " << nr_size << endl;
//   while (is_done == false)
//     {
//       if (nr_size <= 0)
// 	{
// 	  is_done = true;
// 	}
//       else
// 	{
// 	  nr_recv = in.ReadPostEntity(buf, sizeof(buf));
// 	  if (nr_recv == -1)
// 	    is_done = true;
// 	  else
// 	    nr_size -= nr_recv;
// 	}
//     }
//   cout << "generate document ok" << endl;
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
