//
// ioselect.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 25 20:15:24 2005 texane
// Last update Sun Nov 27 00:54:47 2005 texane
//


#ifndef SERVER_IOSLECT_HH
# define SERVER_IOSELECT_HH


#include <sysapi/sysapi.hh>
#include <server/sockioman.hh>


// -
// Simple select based implementation
// of the socket io manager interface


namespace thrman = server;


namespace thrman
{
  class ioselect : public sockioman
  {
    // Handle internet socket io operations by using the select
    // method.

  public:
    
    // Implements the sockioman interface
    bool register_sockhdl(sysapi::socket_in::handle_t&);
    bool unregister_sockhdl(sysapi::socket_in::handle_t&);
    bool write(sysapi::socket_in::handle_t&,
	       sockioman::sockiohandler_t,
	       dataman::buffer*);
    bool read(sysapi::socket_in::handle_t&,
	      sockioman::sockiohandler_t,
	      dataman::buffer*);
    bool close(sysapi::socket_in::handle_t&,
	       sockioman::sockiohandler_t);
    bool timeout(sysapi::socket_in::handle_t&,
		 sockioman::sockiohandler_t);
    bool handle_io();
    bool available_slots(int&);
    bool find_sockio_byhdl(sockioman::sockio_t*&,
			   sysapi::socket_in::handle_t);
    bool done();
    bool& ready();


    ioselect();
    virtual ~ioselect();

    // Accepter/processer/moniter related
    static bool accepter_onread(struct sockio&);
    static bool processer_onread(struct sockio&);
    static bool processer_onwrite(struct sockio&);


  private:
    
    // Maintains sockio structure as a static
    // array of NSOCKIO entries.
    // Allow for 100 connections max.
    enum { NSOCKIO = 100 };
    sockio_t sockios_[NSOCKIO];
    sysapi::socket_in::handle_t hsock_max_;
    int nr_used_;
    bool ready_;
    bool started_;

    // -- system specific code
    //     pthread_t thrid_;
    //     pthread_mutex_t start_lock_;
    //     pthread_cond_t start_cond_;
    //     static void* entry(sockioman*);
    //     sysapi::file::handle_t hfake_[2];
    //     bool unblock_select();

    // Internal sockio management methods
    bool init_sockio(sockioman::sockio_t&, sysapi::socket_in::handle_t&);
    bool release_sockio(sockioman::sockio_t&);
    bool reset_sockio(sockioman::sockio_t&);
    bool searchfor_sockio(sysapi::socket_in::handle_t&, int&);
    bool searchfor_available_sockio(int&);
    bool highest_sockio(sysapi::socket_in::handle_t&);
    bool ioselect_initialize();

    // Internal select related methods
    bool prepare_events(fd_set&, fd_set&);
    bool handle_events(fd_set&, fd_set&);
  };
}


#endif // ! SERVER_IOSELECT_HH
