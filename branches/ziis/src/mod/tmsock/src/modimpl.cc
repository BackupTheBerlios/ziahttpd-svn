//
// modimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 08 14:30:38 2006 texane
// Last update Sat Apr 08 16:43:28 2006 texane
//


#include "include/mod_tmsock.hh"


using namespace std;


// instanciate the module

IModule* GetNewInstance()
{
  return new mod_tmsock;
}


// implement IModule

bool mod_tmsock::ReadConfig(const char* nm_conf)
{
  return false;
}

const char* mod_tmsock::GetModuleName()
{
  return "mod_tmsock";
}

const char* mod_tmsock::GetModuleVersion()
{
  return "1.0";
}

void mod_tmsock::OnLoad(IFS*)
{
}


// implement access monitoring

bool mod_tmsock::enter(SOCKET id_socket, void* p_data)
{
  unsigned long id_self;
  unsigned int i_iodesc;
  unsigned int i_oldest;
  _iodesc_t* p_iodesc;
  bool is_done;

  pthread_mutex_lock(&lk_iodescs);

  // first pass, get a slot:
  // . find a free one
  // . release the oldest
  id_self = get_current_thrid();
  is_done = false;
  i_oldest = 0;
  p_iodesc = 0;
  for (i_iodesc = 0; is_done == false && i_iodesc < mod_tmsock::nr_iodescs; ++i_iodesc)
    {
      if (arr_iodescs[i_iodesc].is_used == true)
	{
	  if (arr_iodescs[i_iodesc].tm_enter < arr_iodescs[i_oldest].tm_enter)
	    i_oldest = i_iodesc;
	}
      else
	{
	  p_iodesc = &arr_iodescs[i_iodesc];
	  is_done = true;
	}
    }
  // no more free slots, release the oldest one
  if (p_iodesc == 0)
    {
      p_iodesc = &arr_iodescs[i_oldest];
//       cout << "[mod_tmsock, " << id_self << "]: @force " << p_iodesc << ", " << p_iodesc->id_thread << endl;
      mod_tmsock::Close(p_iodesc->id_socket, p_data);
    }

  // fill the slot
//   cout << "[mod_tmsock, " << id_self << "]: @enter " << p_iodesc << endl;
  p_iodesc->is_used = true;
  p_iodesc->id_socket = id_socket;
  p_iodesc->id_thread = get_current_thrid();
  sysapi::time::current(p_iodesc->tm_enter);

  // release the lock
  pthread_mutex_unlock(&lk_iodescs);
  return true;
}

bool mod_tmsock::leave()
{
  unsigned long id_self;
  unsigned int i_iodesc;
  _iodesc_t* p_iodesc;
  bool is_done;

  id_self = get_current_thrid();

  pthread_mutex_lock(&lk_iodescs);

  is_done = false;
  p_iodesc = 0;
  for (i_iodesc = 0; is_done == false && i_iodesc < mod_tmsock::nr_iodescs; ++i_iodesc)
    {
      if (arr_iodescs[i_iodesc].is_used == true)
	{
	  if (arr_iodescs[i_iodesc].id_thread == id_self)
	    {
	      p_iodesc = &arr_iodescs[i_iodesc];
	      is_done = true;
	    }
	}
    }

  // release our slot
  if (p_iodesc)
    {
//       cout << "[mod_tmsock, " << id_self << "]: @leave " << p_iodesc << endl;
      p_iodesc->tm_enter = (sysapi::time::handle_t)0;
      p_iodesc->id_thread = 0;
      p_iodesc->id_socket = (SOCKET)-1;
      p_iodesc->is_used = false;
    }
//   else
//     {
//       cout << "[mod_tmsock, " << id_self << "]: @forced " << endl;
//     }

  pthread_mutex_unlock(&lk_iodescs);
  return true;
}
