//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 11:02:05 2006 texane
// Last update Wed Mar 22 16:41:37 2006 texane
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
}

mod_resource::~mod_resource()
{
}


// implement IDocumentGenerator
// {
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
//   if (p_method == 0)
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

//   cout << "generate document ok" << endl;
// }

void mod_resource::GenerateDocument(IInput& in, const char* path, IOutput& out)
{
  ostringstream oss;
  resource::handle* p_resource;
  resource::e_error e_err;
  resource::e_omode e_omode;
  unsigned int nr_size;
  buffer hdr_chunk;
  unsigned int pos_chunk;
  char buf_input[constants::BUFFER_SIZE];
  int nr_input;
  bool is_done;

  // resource creation
  cout << "createing resource" << endl;
  e_omode = resource::O_INPUT;
  if (resource::manager::factory_create(p_resource, resource::ID_FILE, e_omode, path) != E_SUCCESS)
    {
      cout << "cannot create the resource" << endl;
      return ;
    }
  cout << "createing resource done" << endl;

  // main generation loop
  is_done = false;
  pos_chunk = constants::FIRST_CHUNK;

  // if content chunked
  if (p_resource->is_content_dynamic() == true)
    {
      out.SetOutput("transfer-encoding", "chunked");
    }
  else
    {
      p_resource->size(nr_size);
      oss << nr_size;
      out.SetOutput("content-length", oss.str().c_str());
    }

  // send response header
  if (out.SendHeader() == false)
    is_done = true;

  // generate the resource
  while (is_done == false)
    {
      if (p_resource->input_size())
	{
	  nr_input = in.ReadPostEntity(buf_input, sizeof(buf_input));
	  p_resource->flush_input( buffer((unsigned char*)buf_input, (unsigned int)nr_input) );
	}

      e_err = p_resource->generate(nr_size);
      if (e_err == resource::E_SUCCESS)
	{
	  if (p_resource->is_content_dynamic())
	    {
// 	      generate_chunk_header(hdr_chunk, nr_size, pos_chunk);
	      pos_chunk = constants::MIDDLE_CHUNK;
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
	  if (p_resource->is_content_dynamic())
	    {
	      pos_chunk = constants::LAST_CHUNK;
// 	      generate_chunk_header(hdr_chunk, 0, pos_chunk);
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
