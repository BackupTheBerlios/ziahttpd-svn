//
// ziafs_utils.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 14:25:08 2006 texane
// Last update Mon Apr 10 01:29:16 2006 
//


#ifndef UTILS_HH
# define UTILS_HH


#include "buffer.hh"
#include <string>


namespace modutils
{
  class line
  {
  public:
    line();
    ~line();

    // Get next line from a buffer
    bool from_buffer(std::string&, buffer&, bool&);
    bool get_bytes(buffer&);
    bool get_bytes(buffer&, unsigned int);
    bool reset();

//   private:
    // attributes
    buffer m_line;
    buffer m_buf;

    // internal methods
    bool get_from_normal_case();
    bool get_from_special_case();
    bool get_buffer();
  };
}


#endif // ! UTILS_HH
