//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 11:02:05 2006 texane
// Last update Fri Mar 24 14:03:53 2006 texane
//


#include "include/mod_resource.hh"


using namespace std;
using namespace resource;


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
  buffer hdr_chunk;
  bool is_transfer_chunked;
  char buf_input[constants::BUFFER_SIZE];
  const char* te;
  int nr_input;
  bool is_done;

  cout << "[generatedocument]: " << path << endl;

  // resource creation
  p_resource = http_helper::create_resource(in, path, out, conf);
  if (p_resource == 0)
    return ;

  // pre create the resource
  is_done = false;
  is_transfer_chunked = false;

  // handle te
  te = out.GetOutput("transfer-encoding");
  if (te)
    {
      if (stricmp(te, "identity"))
	is_transfer_chunked = true;
    }
  else
    {
      // content is dynamic, chunk the transfer
      if (p_resource->is_content_dynamic() == true)
	{
	  out.SetOutput("transfer-encoding", "chunked");
	  is_transfer_chunked = true;
	}
      else
	{
	  p_resource->size(nr_size);
	  oss << nr_size;
	  out.SetOutput("content-length", oss.str().c_str());
	}
    }

  // send response header
  if (out.SendHeader() == false)
    is_done = true;

  // generate the resource
  while (is_done == false)
    {
      // reset buffers
      hdr_chunk.clear();

      // read and flush to input
      if (p_resource->input_size())
	{
	  nr_input = in.ReadPostEntity(buf_input, sizeof(buf_input));
	  if (nr_input > 0)
	    {
	      p_resource->flush_input( buffer((unsigned char*)buf_input, (unsigned int)nr_input) );
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
	    {
	      p_resource->flush_network(out);
	    }
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
    0
  };

const char** mod_resource::GetSupportedMime()
{
  return _my_mimes;
}
