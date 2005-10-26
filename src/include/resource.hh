//
// resource.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:44:55 2005 
// Last update Wed Oct 26 16:17:22 2005 
//


#ifndef HTTP_DATAMAN_RESOURCE_HH
# define HTTP_DATAMAN_RESOURCE_HH


#include <string>
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
      resource();
      resource(const uri&);
      ~resource();

      // Ressource type related
      std::string type() const;

      // Location related
      const char* universal_name() const;
      const char* local_name() const;
      bool access();

      // Filling {int, out} related
      bool fetch(const uri&);
      bool dump(const uri&);


    private:
      uri	uri_;
      buffer	buf_;

      // - Backward compatibility -
      // Later, use the uri instead of name and path
      char* uriname_;
      char* filename_;
      char** arg_;

      // For the moment, implements resource
      // type as booleans; we should derive later.
      bool cgi_;
      bool raw_;
      bool mod_;

      // method to access resource
      bool local_;
      bool get_;
      bool post_;
      bool put_;
      int errcode_;


      // static 
    };
  }
}


#endif // ! HTTP_DATAMAN_RESOURCE_HH
