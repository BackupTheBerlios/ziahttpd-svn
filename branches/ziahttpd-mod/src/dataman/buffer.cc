//
// buffer.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 23 20:19:10 2005 texane
// Last update Mon Nov 21 23:29:43 2005 texane
//


#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <sysapi/sysapi.hh>
#include <dataman/buffer.hh>
#include <http/uri.hh>


using std::setfill;
using std::setw;
using std::hex;
using std::ostringstream;
using std::string;
using std::cout;
using std::endl;


// helper copy function
static void inline bufcpy(unsigned char* dst, const unsigned char* src, size_t sz)
{
  register size_t n;
  register size_t i;
  register unsigned char* d;
  register const unsigned char* s;

  d = dst;
  s = src;
  n = sz;
  for (i = 0; i < n; ++i)
    d[i] = s[i];
}

// helper 
void dataman::buffer::reset()
{
  if (buf_)
    delete[] buf_;

  buf_ = 0;
  sz_ = 0;
  mmaped_ = false;
}

dataman::buffer::buffer()
{
  buf_ = 0;
  reset();
}


dataman::buffer::buffer(const unsigned char* buf, size_t sz)
{
  buf_ =  0;
  reset();
  buf_ = new unsigned char[sz];
  bufcpy(buf_, buf, sz);
  sz_ = sz;
}


dataman::buffer::buffer(const buffer& b)
{
  buf_ =  0;
  reset();
  *this = b;
}


dataman::buffer::buffer(const http::uri& u)
{
}


dataman::buffer::buffer(sysapi::file::handle_t& hfile)
{
#define NBLK	256
  unsigned char blk[NBLK];
  sysapi::file::size_t nblk;
  register unsigned char* buf;
  unsigned char* sav;
  register int nbuf;
  bool ret;

  buf_ =  0;
  reset();

  nbuf = 0;
  buf = 0;
  while ((ret = sysapi::file::read(hfile, (unsigned char*)blk, sizeof(blk), &nblk)) == true)
    {
      sav = buf;
      buf = new unsigned char[nbuf + nblk];
      if (sav)
	{
	  bufcpy(buf, sav, nbuf);
	  delete[] sav;
	}
      bufcpy(buf + nbuf, blk, nblk);
      nbuf += nblk;
    }

  buf_ = buf;
  sz_ = nbuf;
}


dataman::buffer::~buffer()
{
  reset();
}


size_t	dataman::buffer::size() const
{
  return sz_;
}


char*	dataman::buffer::c_str() const
{
  char* str;
  register char* buf;
  register size_t i;
  register size_t sz;

  str = new char[sz_ + 1];
  sz = sz_;
  buf = reinterpret_cast<char*>(buf_);
  for (i = 0; i < sz; ++i)
    str[i] = buf[i];
  str[i] = 0;
  
  return str;
}


void	dataman::buffer::display() const
{
  size_t i;

  cout << "size: " << static_cast<int>(sz_) << ", ";
  for (i = 0; i < sz_; ++i)
    cout << buf_[i];
  cout << endl;
}


unsigned char* dataman::buffer::dup() const
{
  unsigned char* res;
  
  res = new unsigned char[sz_];
  bufcpy(res, buf_, sz_);
  return res;
}


dataman::buffer dataman::buffer::operator+(const buffer& b)
{
  buffer res(buf_, sz_);

  res.display();
  getchar();

  res += b;
  return res;
}


dataman::buffer& dataman::buffer::operator+=(const buffer& b)
{
  unsigned char* wrk;

  // Copy
  wrk = new unsigned char[sz_ + b.sz_];
  if (buf_)
    {
      bufcpy(wrk, buf_, sz_);
      delete[] buf_;
    }
  if (b.buf_)
    bufcpy(wrk + sz_, b.buf_, b.sz_);

  // affect and release internal buffer
  buf_ = wrk;
  sz_ += b.sz_;

  return *this;
}


dataman::buffer&	dataman::buffer::operator+=(const string& s)
{
  unsigned char* wrk;

  if (!s.size())
    return *this;
  
  // Copy
  wrk = new unsigned char[sz_ + s.size()];
  if (buf_)
    {
      bufcpy(wrk, buf_, sz_);
      delete[] buf_;
    }

  // Append the string content to (buf_, sz_)
  bufcpy(wrk + sz_, reinterpret_cast<const unsigned char*>(s.c_str()), s.size());
  buf_ = wrk;
  sz_ += s.size();

  return *this;
}


dataman::buffer& dataman::buffer::operator=(const buffer& b)
{
  reset();

  sz_ = b.sz_;
  buf_ = new unsigned char[sz_];
  bufcpy(buf_, b.buf_, sz_);
  
  return *this;
}


dataman::buffer& dataman::buffer::operator=(const string& s)
{
  reset();

  if ((sz_ = strlen(s.c_str())))
    {
      buf_ = new unsigned char[sz_];
      bufcpy(buf_, reinterpret_cast<const unsigned char*>(s.c_str()), (size_t)strlen(s.c_str()));
    }

  return *this;
}


unsigned char& dataman::buffer::operator[](int i)
{
  if (!buf_ || (size_t)i < 0 || (size_t)i >= sz_)
    throw (int)0;
  return buf_[i];
}


dataman::buffer::operator unsigned char*()
{
  return buf_;
}


// Mutators, to remove
void	dataman::buffer::buf(unsigned char* buf, size_t sz)
{
  reset();
  buf_ = new unsigned char[sz];
  bufcpy(buf_, buf, sz);
  sz_ = sz;
}


void	dataman::buffer::size(size_t sz)
{
  sz_ = sz;
}


// helper function

inline static char to_printable(char c)
{
  if (c > 32 && c <= 127)
    return c;
  return '.';
}


string	dataman::buffer::to_string(unsigned int windent,
				   unsigned int wborder,
				   unsigned int wspace,
				   unsigned int wstep) const
{
  ostringstream idnt;
  ostringstream wbrd;
  ostringstream wspc;
  ostringstream prnt;
  ostringstream strm;

  for (unsigned int i = 0; i < windent; ++i)
    idnt << ' ';

  for (unsigned int i = 0; i < wborder; ++i)
    wbrd << ' ';

  for (unsigned int i = 0; i < wspace; ++i)
    wspc << ' ';

  for (unsigned int offset = 0; offset < sz_; ++offset)
    {
      if ((offset % wstep) == 0)
	{
	  strm << idnt.str()
	       << "[0x" << setfill('0')
	       << setw(8) << offset
	       << hex << "]"
	       << wbrd.str();
	}

      strm << hex << (unsigned int)buf_[offset];
      prnt << to_printable(buf_[offset]);

      if (((offset + 1) % wstep) == 0)
	{
	  strm << wbrd.str() << prnt.str() << "\n";
	  prnt.str("");
	}
      else
	{
	  strm << wspc.str();
	}
    }

  if ((offset % wstep) != 0)
    {
      for (int nmiss = wstep - offset % wstep; nmiss > 0; --nmiss)
	{
	  strm << "  ";
	  if (nmiss > 1)
	    strm << wspc.str();
	}
      strm << wbrd.str() << prnt.str();
    }

  return strm.str();
}
