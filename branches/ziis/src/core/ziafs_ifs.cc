//
// ziafs_ifs.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Apr 04 14:58:50 2006 texane
// Last update Tue Apr 04 15:05:18 2006 texane
//


#include <ziafs.hh>


int ZfsIfs::IFSOpen(const char* str_filename, const char* str_omode)
{
  return -1;
}


int ZfsIfs::IFSRead(int id_file, long long off_file, char* p_buf, int ln_buf)
{
  return -1;
}


int ZfsIfs::IFSWrite(int id_file, const char* p_buf, int ln_buf)
{
  return -1;
}


void ZfsIfs::IFSClose(int id_file)
{
}


long long ZfsIfs::IFSGetSize(int id_file)
{
  return 0;
}


bool ZfsIfs::IFSRemove(const char* nm_file)
{
  return false;
}
