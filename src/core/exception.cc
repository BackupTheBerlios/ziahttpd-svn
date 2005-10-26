//
// exception.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Wed Oct 26 15:29:43 2005 
// Last update Wed Oct 26 17:43:32 2005 
//

#include <string>
#include <sstream>
#include <server.hh>


using std::string;
using std::ostringstream;


// Execption base interface


// Connection closed exception implementation

string	server::exception::connection_closed::tostring() const
{
  ostringstream strm;
  strm << "<ERROR>: connection close for handle " << (unsigned int)hcon_;
  return strm.str();
}


bool	server::exception::connection_closed::resolve()
{
  return false;
}


server::exception::connection_closed::~connection_closed()
{
  
}
