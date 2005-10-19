//
// session.cc for  in /home/texane/texane/wip/ept3/zia_work/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 19 23:29:57 2005 
// Last update Wed Oct 19 23:44:43 2005 
//


#include <server.hh>


// Construction
server::session::session(sysapi::socket_in::handle_t hdl, server::channel* chan)
{
  hdl_con_ = hdl;
  this_chan_ = chan;
}


// Destruction
server::session::~session()
{
  
}
