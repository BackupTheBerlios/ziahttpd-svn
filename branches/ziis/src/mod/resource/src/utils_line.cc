//
// utils_line.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 14:28:42 2006 texane
// Last update Mon Apr 10 01:53:30 2006 
//


#include "include/mod_resource.hh"


using std::string;


// Internal routines

static char* strcpy_norewind(char *dst, const char *src)
{
  while (*src)
    *dst++ = *src++;
  *dst = 0;
  return (dst);

}

// static char* strcat_norewind(char *dst, const char *src)
// {
//   while (*dst++);
//   while (*src)
//     *dst++ = *src++;
//   *dst = 0;
//   return (dst);
// }

static bool inline is_endof_line(const unsigned char* ptr)
{
  return (*(ptr - 1) == '\r') && (*ptr == '\n');
}

bool modutils::line::get_from_special_case()
{
  unsigned char* ptr;
  unsigned char* src;
  unsigned int bufsz;
  unsigned int sz;

  src = m_buf.bufptr();
  ptr = src + 1;
  bufsz = (unsigned int)m_buf.size() - 1;
  m_buf.resize(bufsz);

  for (sz = 0; sz < bufsz; ++sz)
    *src++ = *ptr++;

  sz = (unsigned int)m_line.size() - 1;
  m_line.resize(sz);
  m_line[sz - 1] = 0;
  
  return true;
}

#include <iostream>

using namespace std;

bool modutils::line::get_from_normal_case()
{
  unsigned int n = 0;
  unsigned int len;
  unsigned int bufsz;
  unsigned char* line;
  bool ret = false;

  if (m_line.size() == 0)
    {
      m_line.resize(1);
      *(m_line.bufptr()) = 0;
    }

  len = (unsigned int)m_line.size() - 1;
  if (len &&
      m_line[len - 1] == '\r' &&
      m_buf.size() &&
      (*(m_buf.bufptr())) == '\n')
    {
      return get_from_special_case();
    }
  else
    {
      bufsz = (unsigned int)m_buf.size();
      if (bufsz)
	{
	  n = 1;
	  while ((n < bufsz) && is_endof_line(m_buf.bufptr() + n) == false)
	    ++n;
	  if (is_endof_line(m_buf.bufptr() + n) == true)
	    {
	      ret = true;
	      --n;
	    }
	}
    }

  // append the line to m_line
  len = n + m_line.size() - 1;
  line = new unsigned char[len + 1];
  memcpy((void*)strcpy_norewind((char*)line, (const char*)m_line.bufptr()), (const void*)m_buf.bufptr(), n);
  line[len] = 0;

  m_line.affect((unsigned char*)line, (size_t)len + 1);

  // resize m_buf
  if (n == m_buf.size())
    {
      m_buf.resize(0);
    }
  else
    {
      unsigned char* ptr = m_buf.bufptr() + n + 2;
      bufsz = (unsigned int)m_buf.size() - 2;
      unsigned int i = 0;
      while (n < bufsz)
	{
	  m_buf[i++] = *ptr++;
	  ++n;
	}
      m_buf.resize(i);
    }

  return ret;
}


// Exported

modutils::line::line()
{}

modutils::line::~line()
{}


bool modutils::line::from_buffer(string& ln, buffer& buf, bool& too_long)
{
  bool ret = false;
  char* c_str;

  // Append to the buffer
  m_buf += buf;
  too_long = false;

  // Try to get a line
  ret = get_from_normal_case();
  if (ret == true)
    {
      c_str = m_line.c_str();
      ln = string(c_str);
      delete[] c_str;
      m_line.clear();
      ret = true;
    }
  else if (m_line.size() >= constants::LINE_SIZE)
    {
      too_long = true;
      ret = false;
    }
  return ret;
}


bool modutils::line::get_bytes(buffer& buf)
{
  return get_bytes(buf, (unsigned int)m_buf.size());
}


bool modutils::line::get_bytes(buffer& buf, unsigned int nbytes)
{
  unsigned int bufsz;

  // normalize the size
  bufsz = nbytes;
  if (nbytes > m_buf.size())
    bufsz = (unsigned int)m_buf.size();

  buf = m_buf;
  buf.resize(bufsz);
  m_buf.remove_front(nbytes);

  return true;
}


bool modutils::line::reset()
{
  m_line.reset();
  return true;
}
