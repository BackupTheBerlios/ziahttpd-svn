//
// thr_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:37 2006 texane
// Last update Wed Feb 15 23:43:08 2006 
//


#include <cstdio>
#include <ziafs.hh>


// todo
// size of recv should
// be configurable.


using namespace sysapi;


// session management

void thr::pool::sess_reset(session_t& sess)
{
  sess.done = false;
  sess.srv = 0;
  sess.thr_slot = 0;
}

void thr::pool::sess_release(session_t& sess)
{
}


// Basic processing stages

bool thr::pool::sess_bind_server(session_t& sess)
{
  error::handle_t herr;

  herr = error::SUCCESS;
  if (sess.srv->is_bound == false)
    {
      sess.srv->is_bound = true;
      herr = insock::create_listening(sess.srv->srv_sock, sess.srv->srv_addr, sess.srv->nr_bklog);
    }
  if (herr == error::SUCCESS)
    return true;
  return false;
}

bool thr::pool::sess_accept_connection(session_t& sess)
{
  error::handle_t herr;

  // Accept a new connection, delegate accept
  herr = insock::accept(sess.cli_sock, sess.cli_addr, sess.srv->srv_sock);
  sess.thr_slot->pool->assign_task(server_entry, (void*)sess.srv);
  if (herr == error::SUCCESS)
    return true;
  return false;
}

bool thr::pool::sess_read_metadata(session_t& sess)
{
  error::handle_t herr;
  unsigned char buf[ZIAFS_STATIC_BUFSZ];
  unsigned int nbytes;

  if (sess.done == true)
    return false;

  herr = recv(*sess.thr_slot, sess.cli_sock, (unsigned char*)buf, sizeof(buf), nbytes);
  if (sess.thr_slot->curr_io.timeouted == true)
    {
      printf("the session timeouted\n");
      sess.done = true;
      return false;
    }
  else if (herr != error::SUCCESS)
    {
      sess.done = true;
      return false;
    }
  buf[nbytes] = 0;
  return true;
}

bool thr::pool::sess_handle_request(session_t& sess)
{
  error::handle_t herr;
  unsigned char buf[ZIAFS_STATIC_BUFSZ];
  unsigned int nbytes;

  if (sess.done == true)
    return false;

  herr = send(*sess.thr_slot, sess.cli_sock, (unsigned char*)buf, sizeof(buf), nbytes);
  if (sess.thr_slot->curr_io.timeouted == true)
    {
      printf("has timeouted\n");
      sess.done = true;
      return false;
    }
  else if (herr != error::SUCCESS)
    {
      sess.done = true;
      return false;
    }
  return true;
}

void* thr::pool::server_entry(thr::pool::slot_t* thr_slot)
{
  // Server's main function
  // thr_slot->uparam contains
  // the server instance.
  // If the server is not yet
  // bound, bind it.

  session_t sess;

  sess_reset(sess);
  sess.thr_slot = thr_slot;
  sess.srv = (net::server*)thr_slot->uparam;

  // session pipeline
  sess_bind_server(sess);
  sess_accept_connection(sess);
  while (sess.done == false)
    {
      sess_read_metadata(sess);
      sess_handle_request(sess);
    }
  sess_release(sess);
  insock::close(sess.cli_sock);

  return 0;
}
