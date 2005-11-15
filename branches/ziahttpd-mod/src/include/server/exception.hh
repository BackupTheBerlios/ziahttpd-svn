//
// exceptions.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 26 15:24:49 2005 
// Last update Sun Nov 13 13:52:27 2005 
//


#ifndef SERVER_EXCEPTION_HH
# define SERVER_EXCEPTION_HH


#include <string>


// This file declares server exceptions interface


namespace server
{
  namespace exception
  {
    class interface
    {
    public:
      virtual std::string	tostring() const = 0;
      virtual bool		solve() = 0;
    };
  }
}


#endif // ! SERVER_EXCEPTION_HH
