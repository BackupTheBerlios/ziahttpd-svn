//
// mmapping.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Mar 24 18:24:17 2006 texane
// Last update Mon Apr  3 15:29:18 2006 
//


#include <string>
#include <sys/sysapi.hh>


using std::string;


sysapi::error::handle_t sysapi::mmapping::alloc(handle_t& h_mmapping, const string& nm_file)
{
  struct stat st_info;
  void* p_map;
  int id_file;
  int err_no;

  id_file = open(nm_file.c_str(), O_RDONLY);
  if (id_file == -1)
    return error::OPEN_FAILED;

  err_no = fstat(id_file, &st_info);
  if (err_no == -1)
    {
      close(id_file);
      return error::OPEN_FAILED;
    }

  p_map = mmap(0, st_info.st_size, PROT_READ, MAP_SHARED, id_file, 0);
  if (p_map == MAP_FAILED)
    {
      close(id_file);
      return error::OPEN_FAILED;
    }

  h_mmapping.id_file = id_file;
  h_mmapping.sz_mmapping = (unsigned long long)st_info.st_size;
  h_mmapping.p_mmapping = (unsigned char*)p_map;
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::mmapping::release(handle_t& h_mmapping)
{
  if (h_mmapping.id_file != -1)
    close(h_mmapping.id_file);
  if (h_mmapping.p_mmapping != MAP_FAILED && h_mmapping.sz_mmapping)
    munmap(h_mmapping.p_mmapping, h_mmapping.sz_mmapping);
  return error::SUCCESS;
}


sysapi::error::handle_t sysapi::mmapping::reset(handle_t& h_mmapping)
{
  h_mmapping.id_file = -1;
  h_mmapping.id_mapping = -1;
  h_mmapping.sz_mmapping = 0;
  h_mmapping.p_mmapping = (unsigned char*)MAP_FAILED;
  return error::SUCCESS;  
}


sysapi::error::handle_t sysapi::mmapping::size(handle_t& h_mmapping, unsigned long long& sz)
{
  sz = h_mmapping.sz_mmapping;
  return error::SUCCESS;  
}
