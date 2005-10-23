//
// buffer.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 23 19:56:39 2005 texane
// Last update Sun Oct 23 23:06:36 2005 texane
//


#ifndef HTTP_DATAMAN_BUFFER_HH
# define HTTP_DATAMAN_BUFFER_HH

#include <sysapi.hh>
#include <cstdlib>

namespace http
{
  namespace dataman
  {
    class buffer
    {
    public:
      buffer();
      buffer(const unsigned char*, size_t);
      buffer(const buffer&);
      buffer(sysapi::file::handle_t&);
      buffer(sysapi::socket_in::handle_t&);

      ~buffer();

      size_t size() const;
      char* c_str() const;
      void display() const;
      unsigned char* dup() const;

      buffer operator+(const buffer&);
      buffer& operator+=(const buffer&);
      buffer& operator=(const buffer&);
      unsigned char& operator[](int);
      operator unsigned char*();

    private:
      void reset();

      bool mmaped_;
      unsigned char* buf_;
      size_t sz_;
    };
  }
}


#endif // ! HTTP_DATAMAN_BUFFER_HH
