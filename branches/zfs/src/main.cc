//
// main.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 01:04:58 2006 texane
// Last update Tue Feb 14 16:29:04 2006 texane
//


#include <ziafs.hh>


int main(int, char**)
{
  thr::pool pool(50);

  pool.assign_task(thr::pool::system_entry, 0);
  getchar();
  while (1)
    {
      pool.assign_task(thr::pool::server_entry, 0);
      Sleep(5000);
    }
}
