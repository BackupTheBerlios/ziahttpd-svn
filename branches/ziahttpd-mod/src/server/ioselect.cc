//
// ioselect.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Fri Nov 25 20:10:53 2005 texane
// Last update Thu Dec 01 13:06:28 2005 texane
//


#include <iostream>
#include <sysapi/sysapi.hh>
#include <server/sockioman.hh>
#include <server/ioselect.hh>
#include <http/session.hh>


using std::cout;
using std::cerr;
using std::endl;


thrman::ioselect::ioselect()
{
  cout << "[*] Ioselect initialization" << endl;
  ioselect_initialize();
  ready_ = true;
}


bool	thrman::ioselect::ioselect_initialize()
{
  // Initialize the io manager.
  // The first slot is always the fake descriptor.

  register int i;

  for (i = 0; i < NSOCKIO; ++i)
    reset_sockio(sockios_[i]);

  nr_used_ = 0;
  hsock_max_ = 0;
  ready_ = false;
  started_ = false;

  // Create a fake descriptor in order to unblock select call
//   {
//     if (pipe(hfake_) == -1)
//       {
// 	perror("cannot pipe");
// 	return false;
//       }
//     register_sockhdl(reinterpret_cast<sysapi::socket_in::handle_t>(hfake_[0]), thrman::sockioman::generic_onread);
//   }

  return true;
}


thrman::ioselect::~ioselect()
{
  register int i;

  for (i = 0; i < NSOCKIO; ++i)
    {
      if (sockios_[i].used_)
	release_sockio(sockios_[i]);
    }

  started_ = false;
  nr_used_ = 0;
  hsock_max_ = 0;

//   pthread_mutex_destroy(&start_lock_);
//   pthread_cond_destroy(&start_cond_);
//   close(hfake_[0]);
//   close(hfake_[1]);
}


// bool	thrman::ioselect::unblock_select()
// {
//   // Deblock the ioman thread, blocked in select
//   // Return true on success, false on error.

//   unsigned char fake = '*';
//   bool ret;

//   ret = sysapi::file::write(hfake_[1], &fake, 1);

//   cout << "\t[*] Select is deblocked" << endl;

//   return ret;
// }


bool	thrman::ioselect::register_sockhdl(sysapi::socket_in::handle_t& hsock)
{
  // Register a new internet socket handle to monitor,
  // along with callback to call on events.
  // Return false if no more slot are available, true on success.

  int i;

  if (searchfor_available_sockio(i) == false)
    return false;

  ++nr_used_;
  if (hsock > hsock_max_)
    hsock_max_ = hsock;

  reset_sockio(sockios_[i]);

  sockios_[i].used_ = true;
  sockios_[i].hsock_ = hsock;
  sockios_[i].onread_ = 0;
  sockios_[i].onwrite_ = 0;
  sockios_[i].onclose_ = 0;
  sockios_[i].ontimeout_ = 0;
  sockios_[i].ioman_ = this;

//   if (started_ == false && ready_ == true)
//     {
//       // The ioman thread is not yet started
//       started_ = true;
//       pthread_cond_signal(&start_cond_);
//     }
//   else if (started_)
//     {
//       // The ioman thread is started and may be blocked
//       // in a call to select.
//       // Deblock for it to reload fdset.
//       unblock_select();
//     }

  return true;
}


bool	thrman::ioselect::unregister_sockhdl(sysapi::socket_in::handle_t& hsock)
{
  int i;
  bool ret;

  if (searchfor_sockio(hsock, i) == false)
    {
      cout << "cannot find" << endl;
      return false;
    }
  
  ret = release_sockio(sockios_[i]);

  cout << "[*] Sockhdl unregistered" << endl;
  return ret;
}


bool	thrman::ioselect::available_slots(int& nr)
{
  nr = NSOCKIO - nr_used_;
  
  if (nr)
    return true;
  return false;
}


bool	thrman::ioselect::prepare_events(fd_set& rdset, fd_set& wrset)
{
  // Fill in the sets to monitor io occuring
  // on socket handles.
  // Always return true.

  register int nev;
  register int i;
  unsigned int ntoprocess;
  
  FD_ZERO(&rdset);
  FD_ZERO(&wrset);
  
  ntoprocess = 0;
  nev = 0;
  i = 0;

  while (nev < nr_used_)
    {
      if (sockios_[i].used_)
	{
	  ++nev;
	  if (sockios_[i].onread_ || sockios_[i].onclose_)
	    {
	      FD_SET(sockios_[i].hsock_, &rdset);
	      ++ntoprocess;
	    }

	  if (sockios_[i].onwrite_)
	    {
	      FD_SET(sockios_[i].hsock_, &wrset);
	      ++ntoprocess;
	    }
	}
      ++i;
    }

  cout << "\t[?] ConnectionCount: " << ntoprocess << endl;

  if (ntoprocess == 0)
    return false;
  return true;
}


bool	thrman::ioselect::handle_events(fd_set& rdset, fd_set& wrset)
{
  // Monitor socket io activity, using select.
  // Always return true.
  // In case the handler to be call
  // wants to redo an operation... that's the reason
  // of the handler variable.
  // here handle interrupted call    

  register int nev;
  register int i;
  bool activio;
  bool closeme;
  sockiohandler_t handler;
  sysapi::socket_in::error_t sockerr;


  cout << "\t[v] Blocked in select, max: " << hsock_max_ << "(+1)" << endl;
  nev = select(hsock_max_ + 1, &rdset, &wrset, 0, 0);
  cout << "\t[^] Blocked in select" << endl;

  if (nev == -1)
    {
      cerr << "[*] error in selecting" << endl;
      sysapi::error::stringify();
      return false;
    }

  // Process timeouted event

  // Process io
  i = 0;
  while (nev > 0)
    {
      // There are no pending io on this socket
      activio = false;
      closeme = false;

      // There is something to read
      if (FD_ISSET(sockios_[i].hsock_, &rdset))
	{
	  activio = true;
	  handler = sockios_[i].onread_;
	  sockios_[i].onread_ = 0;
	  if (handler(sockios_[i].hsock_,
		      sockios_[i].rdbuf_,
		      sockerr) == false)
	    closeme = true;
	}
     
      // There is something to write
      if (FD_ISSET(sockios_[i].hsock_, &wrset) && closeme == false)
	{
	  activio = true;
	  handler = sockios_[i].onwrite_;
	  sockios_[i].onwrite_ = 0;
	  if (handler(sockios_[i].hsock_,
		      sockios_[i].wrbuf_,
		      sockerr) == false)
	    closeme = true;
	}

      // The socket is a one to close
      handler = sockios_[i].onclose_;
      sockios_[i].onclose_ = 0;
      if (closeme && handler)
	handler(sockios_[i].hsock_,
		sockios_[i].rdbuf_,
		sockerr);

      // There has been activities, decrement event number
      if (activio == true)
	--nev;

      // Process next internet socket
      ++i;
    }

  return true;
}


bool	thrman::ioselect::handle_io()
{
  // -- system specific code
  fd_set rdset;
  fd_set wrset;  
  bool ret;

  ret = true;
  if (prepare_events(rdset, wrset) == true)
    {
      // -
      // There are fd to monitor.
      ret = handle_events(rdset, wrset);
    }
  return ret;
}


bool	thrman::ioselect::write(sysapi::socket_in::handle_t& hsock,
				sockioman::sockiohandler_t ophdl,
				dataman::buffer* buf)
{
  // Write to the buffer associated with hsock.
  // Is actually written when the call to select is made.

  int idx;

  if (searchfor_sockio(hsock, idx) == false)
    return false;

  // The callback is already in use
  if (sockios_[idx].onwrite_ != 0)
    return false;

  // Register the callback
  sockios_[idx].onwrite_ = ophdl;
  sockios_[idx].wrbuf_ = buf;

  return true;
}


bool	thrman::ioselect::read(sysapi::socket_in::handle_t& hsock,
			       sockioman::sockiohandler_t ophdl,
			       dataman::buffer* buf)
{
  // Read to the buffer associated with hsock.
  // Is actually written when the call to select is made.

  int idx;

  if (searchfor_sockio(hsock, idx) == false)
    {
      cout << "CANNOT FOUND FOR " << hsock << endl;
      return false;
    }

  // The callback is already in use
  if (sockios_[idx].onread_ != 0)
    return false;

  // Register the callback
  sockios_[idx].onread_ = ophdl;
  sockios_[idx].rdbuf_ = buf;

  return true;
}


bool	thrman::ioselect::close(sysapi::socket_in::handle_t& hsock,
				sockioman::sockiohandler_t ophdl)
{
  // Read to the buffer associated with hsock.
  // Is actually written when the call to select is made.

  int idx;

  if (searchfor_sockio(hsock, idx) == false)
    return false;

  // The callback is already in use
  if (sockios_[idx].onclose_ != 0)
    return false;

  // Register the callback
  sockios_[idx].onclose_ = ophdl;

  return true;
}


bool	thrman::ioselect::timeout(sysapi::socket_in::handle_t& hsock,
				  sockioman::sockiohandler_t ophdl)
{
  // Read to the buffer associated with hsock.
  // Is actually written when the call to select is made.

  int idx;

  if (searchfor_sockio(hsock, idx) == false)
    return false;

  // The callback is already in use
  if (sockios_[idx].ontimeout_ != 0)
    return false;

  // Register the callback
  sockios_[idx].ontimeout_ = ophdl;

  return true;
}


// Internal sockio management methods

bool	thrman::ioselect::init_sockio(sockioman::sockio_t& sio, sysapi::socket_in::handle_t& hsock)
{
  ++nr_used_;

  if (hsock > hsock_max_)
    hsock_max_ = hsock;

  return true;
}


bool	thrman::ioselect::release_sockio(sockioman::sockio_t& sio)
{
  if (sio.used_ == false)
    return false;

  --nr_used_;
  reset_sockio(sio);

  if (sio.hsock_ == hsock_max_)
    {
      if (highest_sockio(hsock_max_) == false)
	hsock_max_ = 0;
    }

  return true;
}


bool	thrman::ioselect::reset_sockio(sockioman::sockio_t& sio)
{
  sio.done_ = false;
  sio.used_ = false;
  sio.onread_ = 0;
  sio.onwrite_ = 0;
  sio.onclose_ = 0;
  sio.ontimeout_ = 0;
  sio.rdbuf_ = 0;
  sio.wrbuf_ = 0;
  sio.ioman_ = 0;
  return true;
}


bool	thrman::ioselect::searchfor_sockio(sysapi::socket_in::handle_t& hsock, int& idx)
{
  register int i;
  register int nr_used;

  nr_used = nr_used_;
  i = 0;

  while (nr_used)
    {
      if (sockios_[i].used_ == true)
	{
	  --nr_used;
	  if (sockios_[i].hsock_ == hsock)
	    {
	      idx = i;
	      return true;
	    }
	}
      ++i;
    }

  return false;
}


bool	thrman::ioselect::find_sockio_byhdl(sockioman::sockio_t*& sio,
					    sysapi::socket_in::handle_t hsock)
{
  int idx;

  if (searchfor_sockio(hsock, idx) == false)
    return false;

  sio = &sockios_[idx];
  return true;
}


bool	thrman::ioselect::searchfor_available_sockio(int& idx)
{
  // Get the next slot available.
  // Return true on success, idx storing the slot index.
  // Return false if no more slot available.

  register int i;

  if (nr_used_ == thrman::ioselect::NSOCKIO)
    return false;

  for (i = 0; i < thrman::ioselect::NSOCKIO; ++i)
    {
      if (sockios_[i].used_ == false)
	{
	  idx = i;
	  return true;
	}
    }

  return false;
}


bool	thrman::ioselect::highest_sockio(sysapi::socket_in::handle_t& hsock)
{
  register int i;
  register int nr_used;
  bool ret;

  ret = false;

  hsock = 0;
  nr_used = nr_used_;
  i = 0;

  while (nr_used)
    {
      if (sockios_[i].used_ == true)
	{
	  --nr_used;
	  ret = true;
	  if (hsock < sockios_[i].hsock_)
	    hsock = sockios_[i].hsock_;
	}
      ++i;
    }

  return ret;
}


bool	thrman::ioselect::done()
{
  if (nr_used_ == 0 && started_ == true)
    return true;
  return false;
}


// void*	thrman::ioselect::entry(sockioman* sio)
// {
//   ioselect* iosel = reinterpret_cast<ioselect*>(sio);

//   pthread_mutex_init(&iosel->start_lock_, 0);
//   pthread_cond_init(&iosel->start_cond_, 0);
//   pthread_mutex_lock(&iosel->start_lock_);

//   iosel->ready_ = true;

//   pthread_cond_wait(&iosel->start_cond_, &iosel->start_lock_);
//   pthread_mutex_unlock(&iosel->start_lock_);

//   while (1)
//     sio->handle_io();

//   return 0;
// }
