//
// buffer.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 23 19:56:39 2005 texane
// Last update Tue Oct 25 19:14:57 2005 
//


#ifndef HTTP_DATAMAN_BUFFER_HH
# define HTTP_DATAMAN_BUFFER_HH

#include <string>
#include <sysapi.hh>
#include <cstdlib>
#include <uri.hh>

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
      explicit buffer(sysapi::file::handle_t&);
      explicit buffer(const uri&);

      ~buffer();

      // mutators, to remove
      void buf(unsigned char*, size_t);

      size_t size() const;
      char* c_str() const;
      void display() const;
      unsigned char* dup() const;

      void reset();

      buffer operator+(const buffer&);
      buffer& operator+=(const buffer&);
      buffer& operator=(const buffer&);
      buffer& operator=(const std::string&);
      unsigned char& operator[](int);
      operator unsigned char*();


    private:

      bool mmaped_;
      unsigned char* buf_;
      size_t sz_;
    };
  }
}


#endif // ! HTTP_DATAMAN_BUFFER_HH
