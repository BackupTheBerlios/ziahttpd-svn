//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 00:16:46 2006 texane
// Last update Wed Jan 25 00:54:56 2006 texane
//


#include <iostream>
#include <string>
#include <core/ziafs.hh>

using std::string;
using namespace net;
using namespace std;

std::list<std::string> ziafs_internal_dump_list;

int main(int ac, char** av)
{
//   string confpath(av[1]);
//   server srv(confpath);
//   srv.process_requests();

  // simple res_insock test
  io::resource* resource;
  io::resource* client;
  buffer* buf;
  bool has_expired;

  // Create a new resource
  resource = new io::res_insock(io::ST_FETCHING, "localhost", 40000);
  ziafs_print_object( *resource );

  inet_utils::init_subsystem();
  resource->io_on_open();
  has_expired = false;
  resource->io_on_read((void*&)client);

  // Basic sequencing flow
  while (has_expired == false)
    {
      resource->io_has_expired(has_expired);
      client->io_on_read((void*&)buf);
      cout << buf->tostring() << endl;
      delete buf;

//       while ()
// 	{
// 	  proto->consum(session, buffer);
// 	  {
// 	    session->target = new io::res...
// 	  }
// 	}
      
    }


  resource->io_on_close();
  inet_utils::release_subsystem();
}
