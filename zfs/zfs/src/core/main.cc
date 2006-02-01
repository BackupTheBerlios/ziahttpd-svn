//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 00:16:46 2006 texane
// Last update Wed Feb 01 02:38:36 2006 texane
//


// on protocl designs
// http://www.itamarst.org/


#include <iostream>
#include <string>
#include <core/ziafs.hh>

using std::string;
using namespace net;
using namespace std;

std::list<std::string> ziafs_internal_dump_list;

int main(int ac, char** av)
{
  server srv(string("fab.xml"));
  srv.process_requests();
}
