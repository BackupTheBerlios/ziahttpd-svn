//
// exceptions.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 26 15:24:49 2005 
// Last update Sun Nov 20 14:38:26 2005 texane
//


#ifndef SERVER_EXCEPTION_HH
# define SERVER_EXCEPTION_HH


#include <iostream>
#include <string>


namespace server
{
  namespace exception
  {
    // Error management is done by
    // exception handling.

    typedef enum
      {
	// Error are enum'd here, used
	// below for template specialization
	CONNECTION = 0,
	MEMORY,
	UNKNOWN
      } error_t;

    class base
    {
      // Base class for error handling
    public:
      bool	report();
      bool	solve();
    };
  }
}


namespace server
{
  namespace exception
  {
    template<error_t EID>
    class error : public base
    {
      // Default error handler

    public:
      bool	report()
      {
	cerr << "Unknown error occured (" << EID << ")" << endl;
	return false;
      }

      bool	solve()
      {
	cerr << "Fatal error, no error handler." << endl;
	exit(-EID);
	return false;
      }
    };
  }
}


#endif // ! SERVER_EXCEPTION_HH
