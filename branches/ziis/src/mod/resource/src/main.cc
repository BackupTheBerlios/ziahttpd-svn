//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 11:02:05 2006 texane
// Last update Wed Mar 23 16:35:16 2005 texane
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
  http_helper::chunk_pos_t pos_chunk;
  char buf_input[constants::BUFFER_SIZE];
  int nr_input;
  bool is_done;

  // resource creation
  p_resource = http_helper::create_resource(in, path, out, conf);
  if (p_resource == 0)
    return ;

  // pre create the resource
  is_done = false;
  is_transfer_chunked = false;
  pos_chunk = http_helper::CHUNK_FIRST;

  if (out.GetOutput("transfer-encoding"))
    {
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

  cout << "sending header" << endl;

  if (out.SendHeader() == false)
    is_done = true;

  // generate the resource
  while (is_done == false)
    {
      // reset buffers
      hdr_chunk.clear();

      if (p_resource->input_size())
	{
	  nr_input = in.ReadPostEntity(buf_input, sizeof(buf_input));
	  if (nr_input > 0)
	    {
	      p_resource->flush_input( buffer((unsigned char*)buf_input, (unsigned int)nr_input) );
	    }
	}

      e_err = p_resource->generate(nr_size);
      if (e_err == resource::E_SUCCESS)
	{
	  if (is_transfer_chunked)
	    {
	      http_helper::generate_chunk_header(hdr_chunk, nr_size, pos_chunk);
	      pos_chunk = http_helper::CHUNK_MIDDLE;
	      p_resource->prepend_header(hdr_chunk);
	    }
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
	      pos_chunk = http_helper::CHUNK_LAST;
 	      http_helper::generate_chunk_header(hdr_chunk, 0, pos_chunk);
	      p_resource->prepend_header(hdr_chunk);
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
    0
  };

const char** mod_resource::GetSupportedMime()
{
  return _my_mimes;
}
