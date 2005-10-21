//
// dataman.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Thu Oct 13 16:09:17 2005 texane
// Last update Fri Oct 21 11:22:56 2005 
//


#include <cstring>
#include <cstring_helper.hh>
#include <sysapi.hh>
#include <dataman.hh>


// - Notes on http requests processing
// In order to be http1.1 compliant, we will process
// client requests as follow:
// . get *crlf
// . get status line
// . get *headers
// . get crlf
// . get body, if any


#include <iostream>


// Concat two block, c-string version
// n: index of the last valid char belonging to the block buffer (src)
#define ISDEL(str) ((*(str - 1) == '\r') && (*(str) == '\n'))


// - ok, special case handling(delimiter is both on dst and src)
static char* _block_cat_special_case(char* dst, char* src)
{
  char* ptr;

  ptr = src + 1;
  while (*ptr)
    *src++ = *ptr++;
  *src = 0;

  dst[(int)strlen(dst) - 1] = 0;
  return dst;
}

static char* _block_cat(char* dst, char* src, bool* done)
{
  char* buf;
  char* ptr;
  int len;
  int n;

  *done = false;

  // Handle the special case, where '\r' is at the end of dst and '\n' starts src
  if (dst && dst[(int)strlen(dst) - 1] == '\r' && *src == '\n')
    {
      *done = true;
      return _block_cat_special_case(dst, src);
    }
  else
    {
      n = 0;
      if (*src)
	{
	  for (n = 1; src[n] && !ISDEL(src + n); ++n)
	    ;
	  if (ISDEL(src + n))
	    {
	      *done = true;
	      --n;
	    }
	}
    }

  // - ok, buf = dst + src[0..n]
  len = n;
  if (dst)
    len += (int)strlen(dst);
  buf = new char[len + 1];
  strncat(cstring_helper::strcpy(buf, dst ? dst : ""), src, n);
  buf[len] = 0;
  if (dst)
    delete[] dst;

  if (!src[n])
    *src = 0;
  else
    {
      ptr = src + n + 2;
      while (*ptr)
	*src++ = *ptr++;
      *src = 0;
    }
  
  return buf;
}

typedef struct	_s_block
{
  bool		used;
  char		*blk;
  char		*res;
  sysapi::socket_in::handle_t hdl;
}		_block_t;


#define _NBLK		256
#define	NR_FDMAX	1024
static _block_t	_block_array[NR_FDMAX];

static inline void unbook_block_entry(int i)
{
  _block_array[i].used = false;
}


static inline void book_block_entry(int i, sysapi::socket_in::handle_t hdl)
{
  _block_array[i].used = true;
  _block_array[i].hdl = hdl;
  _block_array[i].blk = new char[_NBLK + 1];
  _block_array[i].blk[0] = 0;
  _block_array[i].res = NULL;
}

// Find the block associated with handle, or create a new one
static inline _block_t* get_block_entry(sysapi::socket_in::handle_t hdl)
{
  int i;
  int last_not_used = -1;

  // Look for the hdl block, or a free one
  for (i = 0; i < NR_FDMAX; ++i)
    {
      if (_block_array[i].used == false)
	last_not_used = i;
      else
	{
	  if (_block_array[i].hdl == hdl)
	    return &_block_array[i];
	}
    }

  // If I ve no block yet
  if (last_not_used != -1)
    {
      book_block_entry(last_not_used, hdl);
      return &_block_array[last_not_used];
    }

  return NULL;
}


// Get the next http line
bool http::dataman::get_nextline(sysapi::socket_in::handle_t hdl_con, char** ptr_line, sysapi::socket_in::error_t* err)
{
  static bool	_init_me = false;
  _block_t	*ptr;
  sysapi::socket_in::size_t n;
  bool		done;
  bool		ret;

  if (_init_me == false)
    {
      memset(_block_array, 0, NR_FDMAX * sizeof(_block_t));
      _init_me = true;
    }

  ptr = get_block_entry(hdl_con);
  if (ptr == NULL)
    return false;

  ptr->res = _block_cat(ptr->res, ptr->blk, &done);
  if (done == true)
    {
      *ptr_line = ptr->res;
      ptr->res = NULL;
      return true;
    }

//  try_again:
  while ((ret = sysapi::socket_in::recv(hdl_con, reinterpret_cast<unsigned char*>(ptr->blk), _NBLK, &n, err)) == true)
    {
      ptr->blk[n] = 0;
      ptr->res = _block_cat(ptr->res, ptr->blk, &done);
      if (done == true)
	{
	  *ptr_line = ptr->res;
	  ptr->res = NULL;
	  return true;
	}
    }

  if (ret == false)
    {
      // if ((n == -1) && (errno == EINTR))
      // goto try_again;
    }


  // If the client peer has just deconnected, set errno to EPIPE.
  // if (n == 0)
  // errno = EPIPE;

  // sav_errno = errno;
  // delete[] ptr->blk;
  // ptr->blk = NULL;
  // delete[] ptr->res;
  // ptr->res = NULL;
  // ptr->used = false;
  // errno = sav_errno;

  return false;
}


// Get a block from the getnextline buffer
bool http::dataman::get_nextblock(sysapi::socket_in::handle_t hdl_con,
				  unsigned char* buf_body,
				  sysapi::socket_in::size_t sz_body,
				  sysapi::socket_in::size_t* sz_read)
{
  _block_t* ptr;
  sysapi::socket_in::size_t blocklen;
  sysapi::socket_in::size_t i;
  sysapi::socket_in::size_t j;

  *sz_read = 0;

  ptr = get_block_entry(hdl_con);
  if (ptr == NULL)
    return false;

  if (ptr->blk && *ptr->blk)
    {
      blocklen = strlen(ptr->blk);
      *sz_read = sz_body;
      if (blocklen < sz_body)
	*sz_read = blocklen;
      
      for (i = 0; i < *sz_read; ++i)
	buf_body[i] = ptr->blk[i];

      for (j = 0; i < blocklen; ++j, ++i)
	ptr->blk[j] = ptr->blk[i];
      ptr->blk[j] = 0;
    }
  
  return true;
}
