//
// ziafs_utils.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 14:25:08 2006 texane
// Last update Thu Jan 26 19:35:48 2006 texane
//


#ifndef ZIAFS_UTILS_HH
# define ZIAFS_UTILS_HH


#include <ziafs_buffer.hh>
#include <string>


namespace utils
{
  class line
  {
  public:
    line();
    ~line();

    // Get next line from a buffer
    bool from_buffer(std::string&, buffer&);
    bool get_bytes(buffer&);
    bool get_bytes(buffer&, unsigned int);

  private:
    // attributes
    buffer m_line;
    buffer m_buf;

    // internal methods
    bool get_from_normal_case();
    bool get_from_special_case();
    bool get_buffer();
  };
}


#endif // ! ZIAFS_UTILS_HH
