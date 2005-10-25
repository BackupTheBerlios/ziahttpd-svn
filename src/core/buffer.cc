//
// buffer.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 23 20:19:10 2005 texane
// Last update Tue Oct 25 17:34:05 2005 
//

#include <cstring>
#include <string>
#include <iostream>
#include <buffer.hh>
#include <sysapi.hh>


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
void inline http::dataman::buffer::reset()
{
  buf_ = 0;
  sz_ = 0;
  mmaped_ = false;
}

http::dataman::buffer::buffer()
{
  reset();
}


http::dataman::buffer::buffer(const unsigned char* buf, size_t sz)
{
  reset();
  buf_ = new unsigned char[sz];
  bufcpy(buf_, buf, sz);
  sz_ = sz;
}


http::dataman::buffer::buffer(const buffer& b)
{
  reset();
  *this = b;
}


http::dataman::buffer::buffer(const uri& u)
{
}


http::dataman::buffer::buffer(sysapi::file::handle_t& hfile)
{
  reset();
#define NBLK	256
  unsigned char blk[NBLK];
  sysapi::file::size_t nblk;
  register unsigned char* buf;
  unsigned char* sav;
  register int nbuf;
  bool ret;

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


http::dataman::buffer::~buffer()
{
  delete[] buf_;
  reset();
}


size_t http::dataman::buffer::size() const
{
  return sz_;
}


char* http::dataman::buffer::c_str() const
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


void http::dataman::buffer::display() const
{
  size_t i;

  for (i = 0; i < sz_; ++i)
    cout << buf_[i];
  cout << endl;
}


unsigned char* http::dataman::buffer::dup() const
{
  unsigned char* res;
  
  res = new unsigned char[sz_];
  bufcpy(res, buf_, sz_);
  return res;
}


http::dataman::buffer http::dataman::buffer::operator+(const buffer& b)
{
  buffer res(buf_, sz_);

  res.display();
  getchar();

  res += b;
  return res;
}


http::dataman::buffer& http::dataman::buffer::operator+=(const buffer& b)
{
  unsigned char* wrk;

  // copy
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


http::dataman::buffer& http::dataman::buffer::operator=(const buffer& b)
{
  if (buf_)
    delete[] buf_;

  sz_ = b.sz_;
  buf_ = new unsigned char[sz_];
  bufcpy(buf_, b.buf_, sz_);
  
  return *this;
}


http::dataman::buffer& http::dataman::buffer::operator=(const string& s)
{
  if (buf_)
    delete buf_;
  
  sz_ = strlen(s.c_str());
  buf_ = new unsigned char[strlen(s.c_str()) + 1];
  strcpy(reinterpret_cast<char*>(buf_), s.c_str());

  return *this;
}


unsigned char& http::dataman::buffer::operator[](int i)
{
  if (!buf_ || (size_t)i < 0 || (size_t)i >= sz_)
    throw (int)0;
  return buf_[i];
}


http::dataman::buffer::operator unsigned char*()
{
  return buf_;
}
