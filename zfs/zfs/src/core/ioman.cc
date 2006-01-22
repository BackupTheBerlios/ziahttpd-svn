//
// ioman.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Jan 22 02:30:18 2006 texane
// Last update Sun Jan 22 13:52:45 2006 texane
//


#include <string>
#include <core/ziafs_io.hh>
#include <core/ziafs_status.hh>


using std::string;
using namespace status;


error io::manager::create(resource*&, stmask, const string&)
{
  ziafs_return_status(NOTIMPL);
}


error io::manager::destruct(resource* res)
{
  --res->m_refcount;
  if (res->m_refcount == 0)
    {
      delete res;
    }
  ziafs_return_status(SUCCESS);
}
