//
// uri.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 11 17:28:51 2005 texane
// Last update Sun Nov 13 17:44:37 2005 
//


#ifndef HTTP_URI_HH
# define HTTP_URI_HH


#include <string>


namespace http
{
  // Forward declaration
  class session;

  class uri
  {
  public:

    uri();
    uri(session&);

    int& status();

    std::string& localname();
    std::string& widename();
    std::string& extension();


  private:

    int status_;

    std::string localname_;
    std::string widename_;
    std::string extension_;
  };
}



//! \class http::uri
//! \brief Resources naming related
//!
//! UNDOCUMENTED


#endif // ! HTTP_URI_HH
