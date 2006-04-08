//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 13:31:04 2006 texane
// Last update Sat Apr 08 15:25:24 2006 texane
//


#include "include/mod_tmsock.hh"


using namespace std;


mod_tmsock::mod_tmsock(const string& nm_conf)
{
  pthread_mutex_init(&lk_iodescs, 0);
  reload(nm_conf);
}

mod_tmsock::~mod_tmsock()
{
  pthread_mutex_destroy(&lk_iodescs);
}
