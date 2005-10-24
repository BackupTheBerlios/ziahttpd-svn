//
// resource.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:44:55 2005 
// Last update Mon Oct 24 14:56:47 2005 
//


#ifndef HTTP_DATAMAN_RESOURCE_HH
# define HTTP_DATAMAN_RESOURCE_HH


#include <sysapi.hh>
#include <buffer.hh>
#include <uri.hh>


// A resource is named by the client with an uri.
// Handles access checks, and thus http error code;
// Handles internal representation of the resource(memory mmaped...)

// ! Should take the configuration file as parameter


namespace http
{
  namespace dataman
  {
    class resource
    {
    public:
      resource(const uri&);
      ~resource();

      void display() const;

    private:
      const uri& uri_;
      buffer buf_;
    };
  }
}


#endif // ! HTTP_DATAMAN_RESOURCE_HH
