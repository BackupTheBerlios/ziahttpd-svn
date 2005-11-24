//
// resource.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Nov 23 13:04:52 2005 texane
// Last update Thu Nov 24 21:25:54 2005 texane
//


#ifndef DATAMAN_RESOURCE_HH
# define DATAMAN_RESOURCE_HH


#include <vector>
#include <string>
#include <dataman/buffer.hh>
#include <sysapi/sysapi.hh>


namespace dataman
{
  class resource
  {
    // -
    // Resource is an interface

  public:
    // Possible open modes.
    // This is necessary to handle access to resources.
    // For instance, we can have a process whose the stdin
    // has to be feed with the body of an http request.
    typedef enum
      {
	O_FEEDONLY = 0,	// allow feeding operations on resource
	O_FETCHONLY,	// allow fetching operation on resource
	O_FEEDANDFETCH	// allow both
      } openmode_t;

    // Possible resource access failure
    typedef enum
      {
	EOFETCHING = 0,
	EOFEEDING,
	NOTFOUND,
	PERMDENIED,
	SYSLIMIT,
	NOTOPENED,
	OPNOTSUP,
	OPFAILED,
	ALREADYOPENED,
	INUSE
      } error_t;

    // Interface a concrete resource has to implement

    // - Resource interface
    // In order to manipulate resource more generically,
    // and to allow optimizing resource usages,
    // zia core server uses the resource interface.
    // Typically, a cookbook to use a resource is as follow
    // 1. Create a new resource through the resource factory
    // 2. Open the resource when needed
    // 3. Feed the resource until it returns false
    //	  with the error code == EOFEEDING
    // 4. Fetch your buffer with the content produces
    //	  by the resource, until it return false with the
    //    error EOFECHING
    // 5. Close the resource.
    // This way, you can use process, file, buffer in the
    // same way.

    // Open the resource
    virtual bool open(openmode_t, error_t&) = 0;
    // Feed in the resource (think about put with body...)
    virtual bool feed(buffer&, error_t&) = 0;
    // Fetch from resource
    virtual bool fetch(buffer&, unsigned int, error_t&) = 0;
    virtual bool fetch(buffer&, error_t&) = 0;
    // Release resource internals
    virtual bool close(error_t&) = 0;

    // Factory design pattern, by function overloading
    static resource* factory(const std::string&);
    static resource* factory(const std::vector<const std::string>&,
			     const std::vector<const std::string>&);
    static resource* factory(unsigned int);
  };
}


namespace dataman
{
  // -
  // File concrete resource

  class file : public resource
  {
  public:
    file(const std::string&);

    // Implement the resource interface
    bool open(openmode_t, error_t&);
    bool fetch(buffer&, unsigned int, error_t&);
    bool fetch(buffer&, error_t&);
    bool feed(buffer&, error_t&);
    bool close(error_t&);

    // Destructor, close handle if
    // not close.
    virtual ~file();


  private:
    openmode_t omode_;
    bool opened_;
    std::string filename_;
    sysapi::file::handle_t hfile_;
    sysapi::file::size_t sz_;
    
  };
}


namespace dataman
{
  // -
  // Cgi concrete resource

  class cgi : public resource
  {
  public:
    cgi(const std::vector<const std::string>&,
	const std::vector<const std::string>&);

    // Implement the resource interface
    bool open(openmode_t, error_t&);
    bool fetch(buffer&, unsigned int, error_t&);
    bool fetch(buffer&, error_t&);
    bool feed(buffer&, error_t&);
    bool close(error_t&);

    // Destructor, release the process
    virtual ~cgi();

  private:

    // Process creation and status flags
    openmode_t omode_;
    bool allocated_;
    bool feeding_;

    // Resource for the process
    // to be created
    unsigned int ac_;
    char** av_;
    char** env_;

    // Hanlde of the process
    sysapi::process::handle_t hproc_;
    // For feeding
    bool hin_inuse_;
    sysapi::file::handle_t hin_;
    // For fetching
    sysapi::file::handle_t hout_;

    // Internal management methods
    void reset();
    bool release();
  };
}


namespace dataman
{
  // -
  // Report concrete resource.
  // Those are in-memory buffers
  // generated by fly (404 not found...)

  class report : public resource
  {
  public:
    report(unsigned int);

    // Implement the resource interface
    bool open(openmode_t, error_t&);
    bool fetch(buffer&, unsigned int, error_t&);
    bool fetch(buffer&, error_t&);
    bool feed(buffer&, error_t&);
    bool close(error_t&);

    virtual ~report();

  private:

    // Status code
    unsigned int stcode_;
    bool formed_;
    std::string buf_;
  };
}


#endif // ! DATAMAN_RESOURCE_HH
