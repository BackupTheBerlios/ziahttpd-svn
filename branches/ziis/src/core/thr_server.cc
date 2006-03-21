//
// thr_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:37 2006 texane
// Last update Tue Mar 21 23:21:59 2006 texane
//


#include <iostream>
#include <cstdio>
#include <string>
#include <ziafs.hh>


// @info
// There should be a ret_in_cache
// flag, in case where there are no
// more thread in the pool for doing
// accept -> deadlock, since all threads
// are in the wait function.
// Thus, the thread doesn't return in cache

using namespace sysapi;
using namespace std;


// session management

void thr::pool::sess_reset_request(session_t& sess)
{
  sess.done = false;
  sess.proto.reset();
  sess.chunk_pos = net::http::CHUNK_FIRST;
  sess.target = 0;

  // ziis related
  sess.m_input = 0;
  sess.m_output = 0;
  sess.m_comp_module = 0;
  sess.m_comp_data = 0;
  sess.m_gen_module = 0;
  sess.m_modifiers.clear();
}

void thr::pool::sess_release_request(session_t& sess)
{
  core_t* core;

  core = sess.srv->core;
  if (sess.target)
    {
      core->res_manager.factory_destroy(sess.target);
      sess.target = 0;
    }
  if (sess.m_input)
    {
      delete sess.m_input;
      sess.m_input = 0;
    }
  if (sess.m_output)
    {
      delete sess.m_output;
      sess.m_output = 0;
    }
}

void thr::pool::sess_reset(session_t& sess)
{
// +  sysapi::insock::reset_handle(sess.cli_sock);
  sess.done = false;
  sess.srv = 0;
  sess.thr_slot = 0;
  sess.ret_in_cache = true;
  sess_reset_request(sess);
}

void thr::pool::sess_release(session_t& sess)
{
  core_t* core;

  core = sess.srv->core;
  sess_release_request(sess);
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
    {
      sess.m_conn_data = sess.m_conn_module->Accept(sess.cli_sock);
      return true;
    }
  return false;
}

bool thr::pool::sess_read_metadata(session_t& sess)
{
  bool is_valid;
  bool end_of_metadata;
  unsigned char buf[ZIAFS_STATIC_BUFSZ];
  int nr_recv;

  if (sess.done == true)
    return false;

  end_of_metadata = false;
  while (end_of_metadata == false)
    {
      nr_recv = sess.m_conn_module->Recv(sess.cli_sock, sess.m_conn_data, (char*)buf, sizeof(buf));
      if (nr_recv == -1)
	{
	  sess.done = true;
	  return false;
	}
      
      is_valid = sess.proto.consume(buf, nr_recv, end_of_metadata);
      if (is_valid == false)
	{
	  end_of_metadata = true;
	  sess.done = true;
	}
      else if (end_of_metadata == true)
	{
	}

    }

  cout << "end of metadata" << endl;

  // instanciate ZfsInput
  sess.m_input = new ZfsInput(sess.proto);

  // Create the resource
//   sess.proto.create_resource(sess.target, sess.srv->core->res_manager, *sess.srv->srv_config);
  return true;
}


bool thr::pool::sess_handle_predata(session_t& sess)
{
  // handle pre data, such
  // as the expect code...

  buffer buf;
  bool done;
  unsigned int nr_sent;
  sysapi::error::handle_t sys_err;

  if (sess.proto.predata(buf) == true)
    {
      done = false;
      while (done == false)
	{
	  if (buf.size() == 0)
	    {
	      done = true;
	    }
	  else
	    {
	      sys_err = insock::send(sess.cli_sock, buf.bufptr(), (unsigned int)buf.size(), nr_sent);
	      if (sys_err != sysapi::error::SUCCESS)
		{
		  done = true;
		}
	      else
		{
		  buf.remove_front(nr_sent);
		}
	    }
	}
    }
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
  resource::e_error e_err;
  unsigned int nbytes;
  bool done;

  // Basic checks
  if (sess.done == true)
    return false;
  if (sess.target == 0)
    return false;

  done = false;
  // get / post method
  while (done == false)
    {
      if (sess.target->input_size())
	{
	  // Resources can be created with a
	  // buffer as input prefetch.
	  if (sess.target->is_prefetched_input() == true)
	    {
	      sess.target->get_prefetched_input(raw_buf);
	    }
	  else
	    {
	      herr = recv(*sess.thr_slot, sess.cli_sock, (unsigned char*)buf, sizeof(buf), nbytes);
	      if (sess.thr_slot->curr_io.timeouted == true || herr != error::SUCCESS)
		{
		  sess.done = true;
		  return false;
		}
	      raw_buf = buffer((unsigned char*)buf, nbytes);
	    }
	  // Then make http consum/process the buffer(it can be chunked)
	  // +++ sess.proto.consume_body(raw_buf, &body_buf);
	  body_buf = raw_buf;
	  // Send the buffer as input to the resource
	  sess.target->flush_input(*sess.thr_slot, raw_buf);
	}
      // generate the resource
      e_err = sess.target->generate(size);
      if (e_err == resource::E_SUCCESS)
	{
	  // this is for non blocking mode
	  // 	      sess.target->alter(size);
	  sess.proto.create_header(hdr_buf, size, sess.chunk_pos);
	  sess.chunk_pos = net::http::CHUNK_MIDDLE;
	  // 	      sess.proto.modify_header(hdr_buf);
	  sess.target->prepend_header(hdr_buf);
	  if (sess.target->flush_network(*sess.thr_slot, sess.cli_sock) != resource::E_SUCCESS)
	    {
	      sess.done = true;
	      return false;
	    }
	}
      else if (e_err == resource::E_WOULDBLOCK)
	{
	}
      else if (e_err == resource::E_CONTINUE)
	{
// 	  sess.chunk_pos = net::http::CHUNK_MIDDLE;
	}
      else // if (e_err == resource::E_ALREADY_GEN)
	{
	  // Send the last chunk
	  if (sess.proto.response.is_chunk == true)
	    {
	      sess.chunk_pos = net::http::CHUNK_LAST;
	      sess.proto.create_header(hdr_buf, 0, net::http::CHUNK_LAST);
	      sess.target->prepend_header(hdr_buf);
	      sess.target->flush_network(*sess.thr_slot, sess.cli_sock);
	    }
	  done = true;
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
  io_info_reset(sess.thr_slot->curr_io);
  sess.srv = (net::server*)thr_slot->uparam;

  // get the connection module
  if (sess.srv->m_modman.get_connection_module(sess.m_conn_module) == false)
    return 0;

  // session pipeline
  sess_bind_server(sess);
  sess_accept_connection(sess);

  while (sess.done == false)
    {
      sess_reset_request(sess);
      sess_read_metadata(sess);
//       sess_handle_predata(sess);
//       sess_handle_request(sess);
      sess_release_request(sess);
    }
  sess_release(sess);

  if (sess.ret_in_cache == false)
    {
      goto serve_another;
    }

  return 0;
}
