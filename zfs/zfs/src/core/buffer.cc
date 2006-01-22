//
// buffer.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 01:03:32 2006 texane
// Last update Sun Jan 22 16:03:17 2006 texane
//


#include <string>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <core/ziafs_buffer.hh>


using std::setfill;
using std::setw;
using std::hex;
using std::ostringstream;
using std::string;


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
void buffer::reset()
{
  if (buf_)
    delete[] buf_;

  buf_ = 0;
  sz_ = 0;
  mmaped_ = false;
}

buffer::buffer()
{
  buf_ = 0;
  reset();
}


buffer::buffer(const unsigned char* buf, size_t sz)
{
  buf_ =  0;
  reset();
  buf_ = new unsigned char[sz];
  bufcpy(buf_, buf, sz);
  sz_ = sz;
}


buffer::buffer(const buffer& b)
{
  buf_ =  0;
  reset();
  *this = b;
}


buffer::~buffer()
{
  reset();
}


void	buffer::clear()
{
  // Clear the buffer, reset synonym
  return reset();
}


size_t	buffer::size() const
{
  return sz_;
}


char*	buffer::c_str() const
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


unsigned char* buffer::dup() const
{
  unsigned char* res;
  
  res = new unsigned char[sz_];
  bufcpy(res, buf_, sz_);
  return res;
}


buffer buffer::operator+(const buffer& b)
{
  buffer res(buf_, sz_);

  res += b;
  return res;
}


buffer& buffer::operator+=(const buffer& b)
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


buffer&	buffer::operator+=(const string& s)
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


buffer& buffer::operator=(const buffer& b)
{
  reset();

  sz_ = b.sz_;
  buf_ = new unsigned char[sz_];
  bufcpy(buf_, b.buf_, sz_);
  
  return *this;
}


buffer& buffer::operator=(const string& s)
{
  reset();

  if ((sz_ = strlen(s.c_str())))
    {
      buf_ = new unsigned char[sz_];
      bufcpy(buf_, reinterpret_cast<const unsigned char*>(s.c_str()), (size_t)strlen(s.c_str()));
    }

  return *this;
}


unsigned char& buffer::operator[](int i)
{
  if (!buf_ || (size_t)i < 0 || (size_t)i >= sz_)
    throw (int)0;
  return buf_[i];
}


buffer::operator unsigned char*()
{
  return buf_;
}


// Mutators, to remove
void	buffer::buf(unsigned char* buf, size_t sz)
{
  reset();
  buf_ = new unsigned char[sz];
  bufcpy(buf_, buf, sz);
  sz_ = sz;
}


void	buffer::size(size_t sz)
{
  sz_ = sz;
}


// helper function

inline static char to_printable(unsigned char c)
{
  if (c > 32 && c <= 127)
    return c;
  return '.';
}


string	buffer::tostring(unsigned int windent,
			 unsigned int wborder,
			 unsigned int wspace,
			 unsigned int wstep) const
{
  ostringstream idnt;
  ostringstream wbrd;
  ostringstream wspc;
  ostringstream prnt;
  ostringstream strm;
  unsigned int offset;

  for (unsigned int i = 0; i < windent; ++i)
    idnt << ' ';

  for (unsigned int i = 0; i < wborder; ++i)
    wbrd << ' ';

  for (unsigned int i = 0; i < wspace; ++i)
    wspc << ' ';

  for (offset = 0; offset < sz_; ++offset)
    {
      if ((offset % wstep) == 0)
	{
	  strm << idnt.str()
	       << "[0x" << setfill('0')
	       << setw(8) << offset
	       << hex << "]"
	       << wbrd.str();
	}

      strm << hex << setw(2) << setfill('0') << (unsigned int)buf_[offset];
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
