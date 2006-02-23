//
// utils_line.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 14:28:42 2006 texane
// Last update Thu Feb 23 23:01:12 2006 texane
//


#include <string>
#include <ziafs_static.hh>
#include <ziafs_utils.hh>


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

bool utils::line::get_from_special_case()
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

bool utils::line::get_from_normal_case()
{
  unsigned int n = 0;
  unsigned int len;
  unsigned int bufsz;
  unsigned char* line;
  bool ret = false;

  printf("11\n"); fflush(stdout);
  if (m_line.size() == 0)
    {
      printf("22\n"); fflush(stdout);
      m_line.resize(1);
      printf("33\n"); fflush(stdout);
      *(m_line.bufptr()) = 0;
    }

  printf("44\n"); fflush(stdout);
  len = (unsigned int)m_line.size() - 1;
  if (len &&
      m_line[len - 1] == '\r' &&
      m_buf.size() &&
      (*(m_buf.bufptr())) == '\n')
    {
      printf("55\n"); fflush(stdout);
      return get_from_special_case();
    }
  else
    {
      printf("66\n"); fflush(stdout);
      bufsz = (unsigned int)m_buf.size();
      if (bufsz)
	{
	  printf("77\n"); fflush(stdout);
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

  printf("88\n"); fflush(stdout);

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
      unsigned int i = 0;
      bufsz = (unsigned int)m_buf.size();
      while (n < bufsz)
	{
	  m_buf[i++] = *ptr++;
	  ++n;
	}
      m_buf.resize(i - 2);
    }

  return ret;
}


// Exported

utils::line::line()
{}

utils::line::~line()
{}


bool utils::line::from_buffer(string& ln, buffer& buf, bool& too_long)
{
  bool ret = false;
  char* c_str;


  printf("entering mline\n");fflush(stdout);

  // Append to the buffer
  printf("1\n"); fflush(stdout);
  m_buf += buf;
  too_long = false;

  // Try to get a line
  printf("2\n"); fflush(stdout);
  ret = get_from_normal_case();
  printf("3\n"); fflush(stdout);
  if (ret == true)
    {
      printf("4\n"); fflush(stdout);
      c_str = m_line.c_str();
      printf("5\n"); fflush(stdout);
      ln = string(c_str);
      printf("6\n"); fflush(stdout);
      delete[] c_str;
      printf("7\n"); fflush(stdout);
      m_line.clear();
      printf("8\n"); fflush(stdout);
      ret = true;
    }
  else if (m_line.size() >= ZIAFS_STATIC_LINESZ)
    {
      too_long = true;
      ret = false;
    }

  printf("out from mline\n");
  fflush(stdout);

  return ret;
}


bool utils::line::get_bytes(buffer& buf)
{
  return get_bytes(buf, (unsigned int)m_buf.size());
}


bool utils::line::get_bytes(buffer& buf, unsigned int nbytes)
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


bool utils::line::reset()
{
  m_line.reset();
  return true;
}
