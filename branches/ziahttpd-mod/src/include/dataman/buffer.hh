//
// buffer.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Oct 23 19:56:39 2005 texane
// Last update Sun Nov 20 14:49:07 2005 texane
//


#ifndef DATAMAN_BUFFER_HH
# define DATAMAN_BUFFER_HH


#include <string>
#include <cstdlib>
#include <http/uri.hh>
#include <sysapi/sysapi.hh>


namespace dataman
{
  class buffer
  {
  public:
    buffer();
    buffer(const unsigned char*, size_t);
    buffer(const buffer&);
    explicit buffer(sysapi::file::handle_t&);
    explicit buffer(const http::uri&);

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
    buffer& operator+=(const std::string&);
    buffer& operator=(const buffer&);
    buffer& operator=(const std::string&);
    unsigned char& operator[](int);
    operator unsigned char*();

    void prettyprint(int = 0) const;


  private:

    bool mmaped_;
    unsigned char* buf_;
    size_t sz_;
  };
}



//! \file
//! \brief Buffer class
//!
//! Buffers are used to manage memory allocated resources,
//! in order to be able to internally specialize a storage method
//! for a given resouce.
//! Furthermore, centralizing memory managed resources helps
//! in program debugging.

//! \class dataman::buffer
//! \brief buffer class
//!
//! Buffers are used to manage memory allocated resources,
//! in order to be able to internally specialize a storage method
//! for a given resouce.
//! Furthermore, centralizing memory managed resources helps
//! in program debugging.

//! \fn dataman::buffer::buffer(const unsigned char* buf, size_t sz)
//! \brief Fetch a buffer from the c-like buffer passed in argument.
//! 

//! \fn dataman::buffer::buffer(const buffer& b)
//! \brief Fetch a buffer from the buffer passed in argument
//! 

//! \fn dataman::buffer::buffer(sysapi::file::handle_t& hfile)
//! \brief Fetch a buffer from a file
//! 

//! \fn size_t dataman::buffer::size() const
//! \brief return the size of the buffer
//! 

//! \fn char* dataman::buffer::c_str() const
//! \brief return an allocated c-like string
//! 

//! \fn void dataman::buffer::display() const
//! \brief turn the buffer into a human readable form and dump it on std::cout
//! 

//! \fn unsigned char* dataman::buffer::dup() const
//! \brief return a duplicated buffer. Memory is allocated for the new buffer.
//! 

//! \fn void dataman::buffer::reset()
//! \brief Reset the buffer, deallocating memory if already allocated.
//! 

//! \fn dataman::buffer dataman::buffer::operator+(const buffer& b)
//! \brief Return a new buffer form by adding this one and the one passed as argument.
//! 

//! \fn dataman::buffer& dataman::buffer::operator+=(const buffer& b)
//! \brief Add the buffer passed in argument to this one, return this one.
//! 

//! \fn dataman::buffer& dataman::buffer::operator=(const buffer& b)
//! \brief Delete this buffer if already allocated and affect to the one passed as argument.
//! 

//! \fn dataman::buffer::operator unsigned char*()
//! \brief Return a pointer to the internal buffer. Memory is not allocated.
//! 

//! \fn unsigned char& dataman::buffer::operator[](int i)
//! \brief Return the byte at index i in the buffer.
//! 


#endif // ! DATAMAN_BUFFER_HH
