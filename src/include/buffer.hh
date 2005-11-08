//
// buffer.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 23 19:56:39 2005 texane
// Last update Tue Nov  8 13:02:05 2005 
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

      void size(size_t);

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



//! \file
//! \brief Buffer class
//!
//! Buffers are used to manage memory allocated resources,
//! in order to be able to internally specialize a storage method
//! for a given resouce.
//! Furthermore, centralizing memory managed resources helps
//! in program debugging.

//! \class http::dataman::buffer
//! \brief buffer class
//!
//! Buffers are used to manage memory allocated resources,
//! in order to be able to internally specialize a storage method
//! for a given resouce.
//! Furthermore, centralizing memory managed resources helps
//! in program debugging.

//! \fn http::dataman::buffer::buffer(const unsigned char* buf, size_t sz)
//! \brief Fetch a buffer from the c-like buffer passed in argument.
//! 

//! \fn http::dataman::buffer::buffer(const buffer& b)
//! \brief Fetch a buffer from the buffer passed in argument
//! 

//! \fn http::dataman::buffer::buffer(sysapi::file::handle_t& hfile)
//! \brief Fetch a buffer from a file
//! 

//! \fn size_t http::dataman::buffer::size() const
//! \brief return the size of the buffer
//! 

//! \fn char* http::dataman::buffer::c_str() const
//! \brief return an allocated c-like string
//! 

//! \fn void http::dataman::buffer::display() const
//! \brief turn the buffer into a human readable form and dump it on std::cout
//! 

//! \fn unsigned char* http::dataman::buffer::dup() const
//! \brief return a duplicated buffer. Memory is allocated for the new buffer.
//! 

//! \fn void http::dataman::buffer::reset()
//! \brief Reset the buffer, deallocating memory if already allocated.
//! 

//! \fn http::dataman::buffer http::dataman::buffer::operator+(const buffer& b)
//! \brief Return a new buffer form by adding this one and the one passed as argument.
//! 

//! \fn http::dataman::buffer& http::dataman::buffer::operator+=(const buffer& b)
//! \brief Add the buffer passed in argument to this one, return this one.
//! 

//! \fn http::dataman::buffer& http::dataman::buffer::operator=(const buffer& b)
//! \brief Delete this buffer if already allocated and affect to the one passed as argument.
//! 

//! \fn http::dataman::buffer::operator unsigned char*()
//! \brief Return a pointer to the internal buffer. Memory is not allocated.
//! 

//! \fn unsigned char& http::dataman::buffer::operator[](int i)
//! \brief Return the byte at index i in the buffer.
//! 



#endif // ! HTTP_DATAMAN_BUFFER_HH
