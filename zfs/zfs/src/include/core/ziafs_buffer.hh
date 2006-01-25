//
// ziafs_buffer.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Jan 21 23:35:20 2006 texane
// Last update Wed Jan 25 12:13:31 2006 texane
//


#ifndef ZIAFS_BUFFER_HH
# define ZIAFS_BUFFER_HH


#include <string>
#include <cstdlib>


class buffer
{
public:
  buffer();
  buffer(const unsigned char*, size_t);
  buffer(const buffer&);

  ~buffer();

  // mutators, to remove
  void buf(unsigned char*, size_t);

  void resize(size_t);

  size_t size() const;
  char* c_str() const;
  unsigned char* dup() const;

  void reset();
  void clear();

  buffer operator+(const buffer&);
  buffer& operator+=(const buffer&);
  buffer& operator+=(const std::string&);
  buffer& operator=(const buffer&);
  buffer& operator=(const std::string&);
  unsigned char& operator[](int);
  operator unsigned char*();

  std::string tostring(unsigned int = 0,
		       unsigned int = 3,
		       unsigned int = 1,
		       unsigned int = 16) const;
  unsigned char* bufptr();

private:

  bool mmaped_;
  unsigned char* buf_;
  size_t sz_;
};


#endif // ! ZIAFS_BUFFER_HH
