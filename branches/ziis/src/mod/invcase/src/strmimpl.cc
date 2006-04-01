//
// strmimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 01 18:51:58 2006 texane
// Last update Sat Apr 01 19:40:51 2006 texane
//


#include "include/mod_invcase.hh"


int mod_invcase::GetPriority()
{
  return 0;
}


static const char* _arr_mimes[] =
  {
    "text/html",
    0
  };

const char** mod_invcase::GetSupportedContentType()
{
  return _arr_mimes;
}


void mod_invcase::Transform(IBuffer& buf_in, IBuffer& buf_out, IBuffer& buf_remain , bool do_flush)
{
  // this function does nothing but inverting letter case

  unsigned int i_buf;
  unsigned int ln_buf;

  buf_out = buf_in;
  ln_buf = buf_in.Length();
  for (i_buf = 0; i_buf < ln_buf; ++i_buf)
    {
      buf_out[i_buf] = buf_in[i_buf];
      if (buf_in[i_buf] >= 'a' && buf_in[i_buf] <= 'z')
	buf_out[i_buf] -= 'a' - 'A';
      else if (buf_in[i_buf] >= 'A' && buf_in[i_buf] <= 'Z')
	buf_out[i_buf] += 'a' - 'A';
    }
}
