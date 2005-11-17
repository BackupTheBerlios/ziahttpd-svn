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
    std::string& strstatus();

    std::string& localname();
    std::string& widename();
    std::string& extension();


  private:

    int status_;
	std::string strstatus_;

    std::string localname_;
    std::string widename_;
    std::string extension_;
  };
}



//! \class http::uri
//! \brief Resources naming related
//!
//! This call convert the current client query to local file from the hardisk

//! \fn int& http::uri::status()
//! \brief return the status code of the response
//!
//! \return status code

//! \fn std::string& http::uri::strstatus()
//! \brief return the string status code of the response
//!
//! \return string status code

//! \fn std::string& http::uri::localname();
//! \brief return the localname (hard disk file)
//!
//! \return localname

//! \fn std::string& http::uri::widename();
//! \brief return the widename (request asking page)
//!
//! \return widename

//! \fn std::string& http::uri::extension();
//! \brief return the file extension
//!
//! \return extension

#endif // ! HTTP_URI_HH
