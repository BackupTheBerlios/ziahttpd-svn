//
// core.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Oct 11 21:13:49 2005 texane
// Last update Wed Oct 12 17:06:48 2005 texane
//


#ifndef SERVER_CORE_HH
# define SERVER_CORE_HH


#include <list>
#include <session.hh>
#include <channel.hh>

// Here is the basic steps involved in servicing a client request:
// Accept connection
// Read the http request
// Find file
// Send header
// Read the file and send data(can be a looping construct)



namespace server
{
  class core
  {
  public:
    // Construction, destruction
    core();
    ~core();

    // Accessors
    bool done() const { return done_; }

    // Mutators
    void done(bool done) { done_ = done; }

    // Server initialisation
    bool config_init();
    bool config_reset();

  private:
    bool done_;
    bool initialized_;
    
    // Array of channel
//     server::channel* array_channel_;
  };
}


#endif // ! SERVER_CORE_HH
