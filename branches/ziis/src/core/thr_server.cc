//
// thr_server.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 14 15:22:37 2006 texane
// Last update Wed Mar 22 22:43:16 2006 texane
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
  sess.m_conn_module->Close(sess.cli_sock, sess.m_conn_data);
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

  return true;
}


bool thr::pool::sess_handle_document(session_t* sess)
{
  const char* p_hostname;
  string localname;
  string hostname;

  // instanciate ZfsInput
  sess->m_input = new ZfsInput(sess);

  // according to metadata, instanciate the output
  sess->m_output = new ZfsOutput(sess);

  // generate the resource
  p_hostname = sess->m_input->GetInput("host");
  if (p_hostname == 0)
    p_hostname = "";
  hostname = p_hostname;
  localname = sess->proto.get_uri().localname(*sess->srv->srv_config, hostname);
  sess->m_gen_module->GenerateDocument(*sess->m_input, localname.c_str(), *sess->m_output);

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
      sess_handle_document(&sess);
      sess_release_request(sess);
    }
  sess_release(sess);

  if (sess.ret_in_cache == false)
    {
      goto serve_another;
    }

  return 0;
}
