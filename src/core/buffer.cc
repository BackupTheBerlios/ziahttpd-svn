//
// buffer.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 23 20:19:10 2005 texane
// Last update Sun Oct 23 21:47:32 2005 texane
//

#include <iostream>
#include <buffer.hh>


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


http::dataman::buffer::~buffer()
{
  cout << "deleting" << endl;
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
  bufcpy(wrk, buf_, sz_);
  bufcpy(wrk + sz_, b.buf_, b.sz_);

  // affect and release internal buffer
  delete[] buf_;
  buf_ = wrk;
  sz_ += b.sz_;

  return *this;
}


http::dataman::buffer& http::dataman::buffer::operator=(const buffer& b)
{
  delete[] buf_;
  sz_ = b.sz_;
  buf_ = new unsigned char[sz_];
  bufcpy(buf_, b.buf_, sz_);
  
  return *this;
}


unsigned char& http::dataman::buffer::operator[](int i)
{
  return buf_[i];
}


http::dataman::buffer::operator unsigned char*()
{
  return buf_;
}
