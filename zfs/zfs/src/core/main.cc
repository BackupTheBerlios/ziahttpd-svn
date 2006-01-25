//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 00:16:46 2006 texane
// Last update Wed Jan 25 13:59:08 2006 texane
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
  string confpath("");
  server srv(confpath);
  srv.process_requests();
}
