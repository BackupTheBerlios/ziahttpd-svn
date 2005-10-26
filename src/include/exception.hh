//
// exceptions.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 26 15:24:49 2005 
// Last update Wed Oct 26 17:43:23 2005 
//


#ifndef SERVER_EXCEPTION_HH
# define SERVER_EXCEPTION_HH


#include <string>
#include <sysapi.hh>


// This file declares server exceptions interface


namespace server
{
  namespace exception
  {
    class interface
    {
    public:
      virtual std::string	tostring() const = 0;
      virtual bool		resolve() = 0;
    };

    class connection_closed : public interface
    {
    public:
      std::string	tostring() const;
      bool		resolve();

      virtual ~connection_closed();

    private:
      sysapi::socket_in::handle_t hcon_;

    };
  }
}


#endif // ! SERVER_EXCEPTION_HH
