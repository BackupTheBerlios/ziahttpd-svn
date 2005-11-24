//
// exceptions.hh for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 26 15:24:49 2005 
// Last update Thu Nov 24 13:22:52 2005 texane
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
      base(const char* filename = 0,
	   const char* funcname = 0,
	   unsigned int nrline = 0)
      {
	filename_ = "<FILENAME NOT KNOWN>";
	funcname_ = "<FUNCNAME NOT KNOWN>";

	if (filename)
	  filename_ = filename;
	if (funcname_)
	  funcname_ = funcname;
	nrline_ = nrline;
      }

      bool	report()
      {
	std::cerr << "("
		  << filename_ << ","
		  << funcname_ << ","
		  << nrline_
		  << ")"
		  << std::endl;
	std::cerr << "Unknown error occured (<zia-base-exception>)" << std::endl;
	return false;
      }

      bool	solve()
      {
	std::cerr << "Fatal error, no error handler." << std::endl;
	exit(-1);
	return false;
      }


    private:
      const char* funcname_;
      const char* filename_;
      int nrline_;
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
	return base::report();
      }
      bool	solve()
      {
	return base::solve();
      }
    };
  }
}


#endif // ! SERVER_EXCEPTION_HH
