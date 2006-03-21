//
// buffer.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 01:03:32 2006 texane
// Last update Tue Feb 21 17:14:52 2006 texane
//


#include <string>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <ziafs_buffer.hh>


// Todo list
// + test the case on resize, remove_front
// + make unit tests and regression tests


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

unsigned char* buffer::bufptr()
{
  return buf_;
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

  if (buf_ == 0 && sz_)
    {
      printf("!!!!buffer.cc buf is false with a true size\n"); fflush(stdout);
      exit (-1);
    }

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


unsigned char& buffer::operator[](unsigned int i)
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


void	buffer::affect(unsigned char* buf, size_t sz)
{
  reset();
  buf_ = buf;
  sz_ = sz;
}


void	buffer::resize(size_t sz)
{
  if (buf_)
    {
      // First case, buf size < new size
      if (sz > sz_)
	{
	  unsigned char* ptr;
	  ptr = buf_;
	  buf_ = new unsigned char[sz];
	  bufcpy(buf_, ptr, sz);
	  delete[] ptr;
	}
      // Second case, buf size >= sz,
      // do nothing but reduce the size
      // @see end of function.
    }
  else
    {
      buf_ = new unsigned char[sz];
    }
  sz_ = sz;
}


// helper function

inline static unsigned char to_printable(unsigned char c)
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


bool buffer::remove_front(unsigned int nbytes)
{
  unsigned char* to;
  unsigned char* from;
  unsigned int n;

  if (buf_ == 0)
    return false;

  // normalize the size
  if (nbytes > sz_)
    nbytes = (unsigned int)sz_;

  // recopy the buffer
  to = buf_;
  from = buf_ + nbytes;
  for (n = nbytes; n < sz_; ++n)
    *to++ = *from++;
  sz_ -= nbytes;
  return true;
}
