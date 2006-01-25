//
// ziafs_utils.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Jan 25 14:25:08 2006 texane
// Last update Wed Jan 25 15:37:27 2006 texane
//


#ifndef ZIAFS_UTILS_HH
# define ZIAFS_UTILS_HH


#include <core/ziafs_buffer.hh>
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

  private:
    buffer m_line;
    buffer m_buf;

    bool get_from_normal_case();
    bool get_from_special_case();
  };
}


#endif // ! ZIAFS_UTILS_HH
