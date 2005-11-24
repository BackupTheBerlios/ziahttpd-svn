//
// inet_helper.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/include/dataman
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 15:05:06 2005 
// Last update Thu Nov 24 14:44:04 2005 texane
//


#include <iostream>
#include <cstring>
#include <sysapi/sysapi.hh>
#include <dataman/cstring_helper.hh>
#include <dataman/inet_helper.hh>


using namespace dataman;


// This file implements the two basic blocks
// for the http data retrieving.
// Two kind of data: blocks and lines.
// Lines are ended by crlf
// Blocks are sized


// Block size for the recv call
#define SZ_BLOCK	256

// Max connection handles
#define NR_HDLCON	1024

// Block for a given connection handle
typedef struct
{
  sysapi::socket_in::handle_t hdl_con_;
  unsigned char buf_[SZ_BLOCK];
  char* line_;
  sysapi::socket_in::size_t sz_;
  bool used_;
} http_block_t;
static http_block_t gbl_blocks[NR_HDLCON];

// Tell if the gbl_block_array has already been initialized
static bool gbl_blocks_ready = false;

static void inline init_block(http_block_t* blk, sysapi::socket_in::handle_t hdl_con)
{
  blk->line_ = 0;
  blk->used_ = true;
  blk->hdl_con_ = hdl_con;
  blk->sz_ = 0;
}

static void inline release_block(http_block_t* blk)
{
  blk->used_ = false;
  blk->sz_ = 0;
}

// !Fixme: make this function mt safe
static void inline init_all()
{
  memset(gbl_blocks, 0, NR_HDLCON);
  gbl_blocks_ready = true;
}

// !Fixme: make this function mt safe
static void inline release_all()
{
  int i;

  for (i = 0; i < NR_HDLCON; ++i)
    if (gbl_blocks[i].used_ == true)
      release_block(&gbl_blocks[i]);

  gbl_blocks_ready = false;
}

// Get a block or create a new one  if it doesn't yet exist
// !fixme: make me mt safe
// This is the only function calling init_all.. so mt safe made easier
static http_block_t* lookfor_block_byhdl(sysapi::socket_in::handle_t hdl)
{
  int i;
  int last_not_used = -1;

  // init subsystem it not yet done
  if (gbl_blocks_ready == false)
    init_all();

  // Look for the hdl block, or a free one
  for (i = 0; i < NR_HDLCON; ++i)
    {
      if (gbl_blocks[i].used_ == false)
	last_not_used = i;
      else
	{
	  if (gbl_blocks[i].hdl_con_ == hdl)
	    return &gbl_blocks[i];
	}
    }

  // If I ve no block yet
  if (last_not_used != -1)
    {
      init_block(&gbl_blocks[last_not_used], hdl);
      return &gbl_blocks[last_not_used];
    }

  // No more available
  return NULL;
}

// http token related
static bool inline endofline(const char* str)
{
  return (*(str - 1) == '\r') && (*str == '\n');
}

// Remove nr bytes from the begining of the buffer
static void inline bufremove(unsigned char* buf, int bufsz, int nr)
{
  int i;
  int j;

  for (i = 0, j = nr; j < bufsz; ++i, ++j)
    buf[i] = buf[j];
}


// Get a line from the blk->buf_ content, and append it to the blk->line_ content
static bool getline_special_case(http_block_t* blk)
{
  unsigned char* ptr;
  unsigned char* src;
  unsigned int sz;

  src = blk->buf_;
  ptr = src + 1;

  --blk->sz_;
  for (sz = 0; sz < blk->sz_; ++sz)
    *src++ = *ptr++;

  // There is a <CR> at the end of the line
  blk->line_[(int)strlen(blk->line_) - 1] = 0;
  return true;
}

static bool getline(http_block_t* blk)
{
  unsigned int n = 0;
  char* line;
  bool ret = false;
  unsigned int len;

  if (blk->line_ == 0)
    {
      blk->line_ = new char[1];
      *blk->line_ = 0;
    }

  // Handle the special case, delimiter is on the two lines
  if (strlen(blk->line_) && blk->line_[strlen(blk->line_) - 1] == '\r' &&
      blk->sz_ && *blk->buf_ == '\n')
    {
      return getline_special_case(blk);
    }
  else
    {
      if (blk->sz_)
	{
	  n = 1;
	  while ((n < blk->sz_) && (endofline((const char*)(blk->buf_ + n)) == false))
	    ++n;
	  if (endofline((const char*)(blk->buf_ + n)) == true)
	    {
	      ret = true;
	      --n;
	    }
	}
    }

  // put the whole line in line, and then in blk->line_
  len = n + strlen(blk->line_);
  line = new char[len + 1];
  memcpy((void*)cstring_helper::strcpy(line, blk->line_), (const void*)blk->buf_, n);
  line[len] = 0;

  delete[] blk->line_;
  blk->line_ = line;

  // resize the block
  if (n == blk->sz_)
    blk->sz_ = 0;
  else
    {
      unsigned char* ptr = &blk->buf_[n + 2];
      int i = 0;
      while (n < blk->sz_)
	{
	  blk->buf_[i++] = *ptr++;
	  ++n;
	}
      blk->sz_= i - 2;
    }

  return ret;
}


// --
// Unit entry points

bool	dataman::get_nextline(sysapi::socket_in::handle_t hdl_con,
			      char** line,
			      sysapi::socket_in::error_t* err)
{
  http_block_t* blk;
  sysapi::socket_in::size_t sz;
  bool ret;

  blk = lookfor_block_byhdl(hdl_con);
  if (blk == NULL)
    return false;

  if (getline(blk) == true)
    {
      *line = blk->line_;
      blk->line_ = 0;
      return true;
    }

  // feed the buffer until we got a new line
  while ((ret = recv(hdl_con, blk->buf_, SZ_BLOCK, &sz, err)) == true)
    {
      blk->sz_ = sz;
      if (getline(blk) == true)
	{
	  *line = blk->line_;
	  blk->line_ = 0;
	  return true;
	}
    }

  return false;
}


bool	dataman::get_nextblock(sysapi::socket_in::handle_t hdl_con,
			       unsigned char** buf,
			       sysapi::socket_in::size_t blocksz,
			       sysapi::socket_in::size_t* recvsz,
			       sysapi::socket_in::error_t* err)
{
  http_block_t* blk;

  blk = lookfor_block_byhdl(hdl_con);
  if (blk == NULL)
    return false;

  *buf = new unsigned char[blocksz];
  *recvsz = 0;
  
  if (blk->sz_ < blocksz)
    {
      sysapi::socket_in::size_t nr_missing;
      sysapi::socket_in::size_t nr_recv;
      int i;
      
      nr_missing = blocksz - blk->sz_;
      i = blk->sz_;
      while (--i >= 0)
	(*buf)[i] = blk->buf_[i];
      sysapi::socket_in::recv(blk->hdl_con_, (*buf) + blk->sz_, nr_missing, &nr_recv, err);
      *recvsz = blk->sz_ + nr_recv;
      blk->sz_ = 0;
    }
  else
    {
      memcpy((void*)*buf, blk->buf_, blocksz);
      bufremove(blk->buf_, blk->sz_, blocksz);
      *recvsz = blocksz;
      blk->sz_ -= blocksz;
    }

  return true;
}
