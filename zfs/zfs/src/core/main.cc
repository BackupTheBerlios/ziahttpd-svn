//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 00:16:46 2006 texane
// Last update Wed Jan 25 12:05:46 2006 texane
//


#include <iostream>
#include <string>
#include <core/ziafs.hh>

using std::string;
using namespace net;
using namespace std;

std::list<std::string> ziafs_internal_dump_list;


static void global_ctor()
{
  sysapi::insock::init_subsystem();
}


static void global_dtor()
{
  sysapi::insock::release_subsystem();
}

int main(int ac, char** av)
{
//   string confpath(av[1]);
//   server srv(confpath);
//   srv.process_requests();

  // simple res_insock test
  io::resource* resource;
  io::resource* client;
  buffer* buf;
  bool has_expired = false;

  // do global construction of the server
  global_ctor();

  // Create a new resource
  resource = new io::res_insock(io::ST_FETCHING, "localhost", 40000);
  ziafs_print_object( *resource );
  resource->io_on_open();
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


  // do global destruction of the server
  global_dtor();
}
