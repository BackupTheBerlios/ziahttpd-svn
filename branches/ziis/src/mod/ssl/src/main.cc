//
// main.cc for  in /home/texane/texane/projects/ziahttpd/branches/ziis/src/mod/ssl
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Apr  9 03:17:26 2006 
// Last update Sun Apr  9 04:08:41 2006 
//


#include "include/modimpl.hh"


// @read this
// http://www.linuxjournal.com/article/4822


using namespace std;

extern void print_ssl_error();

void mod_ssl::reset()
{
  m_port = 0;
  m_host.clear();
  m_config = 0;
  m_cert_file = SSL_CERT;
  m_privkey_file = SSL_PRIVKEY;
}

void mod_ssl::release()
{
  if (m_config)
    {
      delete m_config;
      m_config = 0;
    }
}

bool mod_ssl::reload(const string& nm_conf)
{
  list<utils::config::certif*>::iterator it_certif;

  release();
  reset();

  // get certificate
  m_config = new utils::config(nm_conf.c_str());
  m_config->get_certif(it_certif);
  if (m_config->end_certif(it_certif) == false)
    {
      m_cert_file = (*it_certif)->pem_certif;
      m_privkey_file = (*it_certif)->pem_privkey;
    }
  return true;
}

mod_ssl::mod_ssl(const string& nm_conf)
{
  int nr_ret;

  // reload config file
  reset();
  ReadConfig(nm_conf.c_str());

  // reset
  m_ssl_method = 0;
  m_ssl_context = 0;

  // load ssl error strings
  SSL_library_init();
  SSL_load_error_strings();

  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();

  // init ssl context
  m_ssl_method = SSLv23_server_method();
  if (m_ssl_method == 0)
    {
      print_ssl_error();
      return ;
    }

  m_ssl_context = SSL_CTX_new(m_ssl_method);
  if (m_ssl_context == 0)
    {
      print_ssl_error();
      return ;
    }

  SSL_CTX_set_mode(m_ssl_context, SSL_MODE_AUTO_RETRY);
  nr_ret = SSL_CTX_use_certificate_chain_file(m_ssl_context, SSL_CERT);
  if (nr_ret == 1)
    nr_ret = SSL_CTX_use_RSAPrivateKey_file(m_ssl_context, SSL_PRIVKEY, SSL_FILETYPE_PEM);

  if (nr_ret <= 0)
    print_ssl_error();
}

mod_ssl::~mod_ssl()
{
  if (m_config)
    delete m_config;
  ERR_free_strings();
  EVP_cleanup();
  SSL_CTX_free(m_ssl_context);
}
