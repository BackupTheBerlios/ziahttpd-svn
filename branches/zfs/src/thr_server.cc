//
// thr_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:37 2006 texane
// Last update Fri Feb 17 16:57:40 2006 texane
//


#include <cstdio>
#include <string>
#include <iostream>
#include <ziafs.hh>


// @info
// There should be a ret_in_cache
// flag, in case where there are no
// more thread in the pool for doing
// accept -> deadlock, since all threads
// are in the wait function.
// Thus, the thread doesn't return in cache

using namespace sysapi;


// session management

void thr::pool::sess_reset(session_t& sess)
{
  sess.done = false;
  sess.srv = 0;
  sess.thr_slot = 0;
  sess.ret_in_cache = true;
  sess.proto.reset();
  sess.target = 0;
}

void thr::pool::sess_release(session_t& sess)
{
  core_t* core;

  core = sess.srv->core;
  if (sess.target)
    core->res_manager.factory_destroy(sess.target);
  if (sess.thr_slot->curr_io.timeouted == false)
    insock::close(sess.cli_sock);
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
  if (sess.thr_slot->pool->assign_task(server_entry, (void*)sess.srv) == false)
    sess.ret_in_cache = false;
  if (herr == error::SUCCESS)
    return true;
  return false;
}

bool thr::pool::sess_read_metadata(session_t& sess)
{
  bool valid;
  bool end_of_metadata;
  error::handle_t herr;
  unsigned char buf[ZIAFS_STATIC_BUFSZ];
  unsigned int nbytes;

  if (sess.done == true)
    return false;

  end_of_metadata = false;
  while (end_of_metadata == false)
    {
      herr = recv(*sess.thr_slot, sess.cli_sock, (unsigned char*)buf, sizeof(buf), nbytes);
      if (sess.thr_slot->curr_io.timeouted == true)
	{
	  sess.done = true;
	  return false;
	}
      else if (herr != error::SUCCESS)
	{
	  sess.done = true;
	  return false;
	}

      valid = sess.proto.consume(buf, nbytes, end_of_metadata);
      if (valid == false)
	{
	  end_of_metadata = true;
	  sess.done = true;
	}
      else if (end_of_metadata == true)
	{
	}

    }

  // Create the resource
//   sess.srv->core->res_manager.factory_create(sess.target,
// 					     resource::ID_BYFLY,
// 					     resource::O_INPUT,
// 					     "toto");
  sess.srv->core->res_manager.factory_create(sess.target,
					     resource::ID_FILE,
					     resource::O_INPUT,
					     "out.html");

  return true;
}


bool thr::pool::sess_handle_request(session_t& sess)
{
  buffer raw_buf;
  buffer body_buf;
  buffer hdr_buf;
  unsigned int size;
  unsigned char buf[ZIAFS_STATIC_BUFSZ];
  sysapi::error::handle_t herr;
  unsigned int nbytes;
  bool done;

  // Basic checks
  if (sess.done == true)
    return false;
  if (sess.target == 0)
    return false;

  done = false;
  if (sess.target->is_input() == true)
    {
      // get / post method
      while (done == false)
	{
 	  if (sess.proto.body_size())
	    {
	      printf("in body size!!!\n"); fflush(stdout);
	      // First read the body next buffer
	      herr = recv(*sess.thr_slot, sess.cli_sock, (unsigned char*)buf, sizeof(buf), nbytes);
	      if (sess.thr_slot->curr_io.timeouted == true || herr != error::SUCCESS)
		{
		  sess.done = true;
		  return false;
		}
	      // Then make http consum/process the buffer(it can be chunked)
 	      sess.proto.consume_body(raw_buf, &body_buf);
	      // Send the buffer as input to the resource
	      sess.target->flush_input(*sess.thr_slot, raw_buf);
	    }
	  if (sess.target->generate(size) == resource::E_SUCCESS)
	    {
// 	      sess.target->alter(size);
// 	      sess.proto.create_header(hdr_buf, chunked);
	      sess.target->prepend_header(hdr_buf);
	      if (sess.target->flush_network(*sess.thr_slot, sess.cli_sock) != resource::E_SUCCESS)
		{
		  sess.done = true;
		  return false;
		}
	    }
	  else
	    {
	      done = true;
	    }
	}
    }
  else if (sess.target->is_output() == true)
    {
      // This is a put method
      while (done == false)
	{
	  if (sess.proto.body_size() == 0)
	    {
	      done = true;
	    }
	  else
	    {
	    }
	}
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

 serve_another:
  sess_reset(sess);
  sess.thr_slot = thr_slot;
  sess.srv = (net::server*)thr_slot->uparam;
  io_info_reset(sess.thr_slot->curr_io);

  // session pipeline
  sess_bind_server(sess);
  sess_accept_connection(sess);
  while (sess.done == false)
    {
      sess_read_metadata(sess);
      sess_handle_request(sess);
    }
  sess_release(sess);

  if (sess.ret_in_cache == false)
    {
      goto serve_another;
    }

  return 0;
}
