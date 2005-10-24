//
// uri.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Mon Oct 24 13:46:22 2005 
// Last update Mon Oct 24 14:37:52 2005 
//


#ifndef HTTP_DATAMAN_URI_HH
# define HTTP_DATAMAN_URI_HH


// Name a resource from the client stand point
// For the server standpoint, tell the actual location
// of the resource.
// The only goal of this class is to map the universal name
// to a local one ACCORDING TO THE CONFIGURATION FILE.
// In order to do so, it must take a configuration as argument.
// Access checks is delegated to the resource class.

namespace http
{
  namespace dataman
  {
    class uri
    {
    public:
      uri(const char*);
      ~uri();

      void map(const char* = 0);
      void map();

      const char* universal_name() const;
      const char* local_name() const;

    private:
      char* universal_name_;
      char* local_name_;

      // const config& conf_;
    };
  }
}


#endif // ! HTTP_DATAMAN_URI
