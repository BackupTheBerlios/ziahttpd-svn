//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 11:02:05 2006 texane
// Last update Mon Apr 03 19:50:56 2006 texane
//


#include "include/mod_resource.hh"


using namespace std;
using namespace resource;


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



// module management

IModule* GetNewInstance()
{
  return new mod_resource;
}

mod_resource::mod_resource()
{
  conf = NULL;
}

mod_resource::~mod_resource()
{
}


void mod_resource::GenerateDocument(IInput& in, const char* path, IOutput& out)
{
  ostringstream oss;
  resource::handle* p_resource;
  resource::e_error e_err;
  unsigned int nr_size;
  bool is_transfer_chunked;
  char buf_input[constants::BUFFER_SIZE];
  const char* te;
  int nr_input;
  bool is_done;

  // resource creation
  p_resource = http_helper::create_resource(in, path, out, conf);
  if (p_resource == 0)
    return ;

  // pre create the resource
  is_done = false;
  is_transfer_chunked = false;
  te = out.GetOutput("transfer-encoding");

  // chunk but DONT encode if dynamic content
  if (p_resource->is_content_dynamic() == true)
    {
      out.SetOutput("content-length", "");
      out.SetOutput("content-encoding", "");
      out.SetOutput("transfer-encoding", "chunked");
      is_transfer_chunked = true;
    }
  else if (te && !stricmp(te, "chunked"))
    {
      out.SetOutput("content-length", "");
      is_transfer_chunked = true;
    }
  else
    {
      p_resource->size(nr_size);
      oss << nr_size;
      out.SetOutput("content-length", oss.str().c_str());
    }

  // debug resource
//   {
//     cout << GetCurrentThreadId() << ": [generatedocument]: " << path << endl;
//     cout << "te: chunked ==  " << is_transfer_chunked << endl;
//     cout << "resource input size == " << p_resource->input_size() << endl;
//     if (in.GetInput("content-length"))
//       cout << "input hash value == " << in.GetInput("content-length") << endl;
//     else
//       cout << "input hash value == " << 0 << endl;
//   }

  // send response header
  if (out.SendHeader() == false)
    is_done = true;

  // generate the resource
  while (is_done == false)
    {
      // read and flush to input
      if (p_resource->input_size())
	{
	  nr_input = in.ReadPostEntity(buf_input, sizeof(buf_input));
	  if (nr_input > 0)
	    {
	      buffer buf_toflush((unsigned char*)buf_input, (unsigned int)nr_input);
	      p_resource->flush_input( buf_toflush );
	    }
	}

      // generate the resource
      e_err = p_resource->generate(nr_size);
      if (e_err == resource::E_SUCCESS)
	{
	  if (p_resource->flush_network(out) != resource::E_SUCCESS)
	    is_done = true;
	}
      else if (e_err == resource::E_WOULDBLOCK)
	{
	}
      else if (e_err == resource::E_CONTINUE)
	{
	}
      else // ALREADY_GEN
	{
	  // send the last chunk
	  if (is_transfer_chunked)
	    p_resource->flush_network(out);
	  is_done = true;
	}
    }

  // resource deletion
  resource::manager::factory_destroy(p_resource);
}


// exported mime types

static const char* _my_mimes[] =
  {
    "text/html",
    "image/gif",
    "image/png",
    "application/pdf",
    0
  };

const char** mod_resource::GetSupportedMime()
{
  return _my_mimes;
}
