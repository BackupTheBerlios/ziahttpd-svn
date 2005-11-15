//
// msgdata.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 11 17:29:32 2005 texane
// Last update Sun Nov 13 16:04:05 2005 
//


#ifndef HTTP_MSGDATA_HH
# define HTTP_MSGDATA_HH


#include <string>
#include <http/uri.hh>
#include <dataman/buffer.hh>


namespace http
{
  class msgdata
  {

  public:
    ~msgdata();

    std::string& operator[](const std::string&);

    bool parse_rqstline(dataman::buffer&, const uri&);
    bool build_respline(msgdata& rqst, dataman::buffer& metada, const uri&);

    std::string& query_string();
    
  private:
  };
}


//! \class http::msgdata
//! \brief Http messages data manipulation
//!
//! UNDOCUMENTED



#endif // !HTTP_MSGDATA_HH
