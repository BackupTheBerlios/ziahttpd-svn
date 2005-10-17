//
// client.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Oct 11 21:17:59 2005 texane
// Last update Wed Oct 12 12:20:39 2005 texane
//


#ifndef SERVER_CLIENT_HH
# define SERVER_CLIENT_HH


#include <sysapi.hh>



typedef struct
{
  int tm_max_;
  int tm_expire_;
  int nr_rqust_max_;
  int nr_rqust_cur_;
} http_client_t;


namespace server
{
  class client
  {
    
  public:
    // Construction, destruction
    client();
    ~client();

  private:
    // Working thread for this client
    sysapi::thread::handle_t thr_hdl_;
    // Mutex on the accept call for the thread to block
    // sysapi::mutex::handle_t accept_lock_;
    // sysapi::sockin::handle_t accept_hdl_;

    // Structure describing client socket connection
//     server::insock_t ;
//     struct sockin
//     {
//       sysapi::socket::handle_t hdl_con_;
//       struct sockaddr sockaddr_; 
//     } sockin_;

    // Client entry point, receives its own address as parameter
    static sysapi::thread::retcode_t client_entry(sysapi::thread::param_t);
  };
}


#endif // ! SERVER_CLIENT_HH
