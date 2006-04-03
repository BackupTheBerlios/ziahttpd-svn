//
// conimpl.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 01 11:14:10 2006 texane
// Last update Mon Apr  3 19:59:57 2006 
//


#include "include/modimpl.hh"


using namespace std;


static void reset_ssl_data(mod_ssl::_ssl_data_t* p_ssl_data)
{
  p_ssl_data->m_bio = 0;
  p_ssl_data->m_ssl = 0;
}

void print_ssl_error()
{
  cout << ERR_error_string(ERR_get_error(), NULL) << endl;
}


// implement IConnection

short mod_ssl::GetPort()
{
  return m_port;
}

const char* mod_ssl::GetHost()
{
  return m_host.c_str();
}

void* mod_ssl::Accept(SOCKET id_socket)
{
  mod_ssl::_ssl_data_t* p_ssl_data;
  int val_accept;

  cout << pthread_self() << ": [module] module->Accept()" << "on socket " << (int)id_socket << endl;

  // checks
  if (m_ssl_context == 0)
    return 0;

  p_ssl_data = new mod_ssl::_ssl_data_t;
  reset_ssl_data(p_ssl_data);
  p_ssl_data->m_bio = BIO_new_socket((int)id_socket, BIO_NOCLOSE);
  if (p_ssl_data->m_bio == 0)
    return 0;

  // create the ssession and set the bio
  p_ssl_data->m_ssl = SSL_new(m_ssl_context);
  if (p_ssl_data->m_ssl == 0)
    return 0;
  SSL_set_bio(p_ssl_data->m_ssl, p_ssl_data->m_bio, p_ssl_data->m_bio);

  // accept the connection(handshake)
  val_accept = SSL_accept(p_ssl_data->m_ssl);
  if (val_accept <= 0)
    {
      print_ssl_error();
      BIO_free_all(p_ssl_data->m_bio);
      return 0;
    }

  return (void*)p_ssl_data;
}

int mod_ssl::Recv(SOCKET h_sock, void* p_data, char* p_buf, int ln_buf)
{
  int nr_read;
  mod_ssl::_ssl_data_t* p_ssl_data;

  p_ssl_data = (mod_ssl::_ssl_data_t*)p_data;
  if (p_ssl_data == 0)
    return -1;

//   SSL_set_accept_state(p_ssl_data->m_ssl);

 bio_read_again:
  nr_read = SSL_read(p_ssl_data->m_ssl, p_buf, ln_buf);
  if (nr_read <= 0)
    {
      if (BIO_should_retry(p_ssl_data->m_bio))
	goto bio_read_again;
      return -1;
    }
  
  return nr_read;
}

int mod_ssl::Send(SOCKET h_sock, void* p_data, const char* p_buf, int ln_buf)
{
  int nr_sent;
  mod_ssl::_ssl_data_t* p_ssl_data = (mod_ssl::_ssl_data_t*)p_data;

  if (p_data == 0)
    return -1;

//   SSL_set_accept_state(p_ssl_data->m_ssl);

 bio_send_again:
  nr_sent = SSL_write(p_ssl_data->m_ssl, p_buf, ln_buf);
  if (nr_sent <= 0)
    {
      if (BIO_should_retry(p_ssl_data->m_bio))
	goto bio_send_again;
      return -1;
    }
  
  return nr_sent;
}

void mod_ssl::Close(SOCKET id_socket, void* p_data)
{
  int val_shutdown;

  cout << pthread_self() << ": [ssl_mod]->Close()" << endl;

  _ssl_data_t* p_ssl_data = (_ssl_data_t*)p_data;
  if (p_data == 0)
    return ;

  // send tcp_fin or get client notify
  val_shutdown = SSL_shutdown(p_ssl_data->m_ssl);
  if (val_shutdown == 0)
    {
      shutdown(id_socket, 1);
      val_shutdown =  SSL_shutdown(p_ssl_data->m_ssl);
    }

  // release internals
  if (p_ssl_data->m_bio)
    BIO_free_all(p_ssl_data->m_bio);

  delete p_ssl_data;
}
