//
// main.cc<2> for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Mar 21 13:31:04 2006 texane
// Last update Sat Apr 01 16:58:31 2006 texane
//


#include "include/modimpl.hh"


// @read this
// http://www.linuxjournal.com/article/4822


using namespace std;


// internal management

static void print_ssl_error()
{
  cout << ERR_error_string(ERR_get_error(), NULL) << endl;
}


void mod_ssl::reset()
{
  m_port = 0;
  m_host.clear();
}

void mod_ssl::release()
{
}

bool mod_ssl::reload(const string& nm_conf)
{
  release();
  reset();
  return true;
}

mod_ssl::mod_ssl(const string& nm_conf)
{
  reload(nm_conf);

  // reset
  m_ssl_method = 0;
  m_ssl_context = 0;

  // load ssl error strings
  SSL_library_init();
  SSL_load_error_strings();
  ERR_load_BIO_strings();
  OpenSSL_add_all_algorithms();

  // init ssl context
  m_ssl_method = SSLv23_method();
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

  cout << m_ssl_context << endl;

  if (SSL_CTX_use_certificate_chain_file(m_ssl_context, "C:\\home\\texane\\ziafs\\branches\\ziis\\conf\\ssl_certs\\cacert.pem") != 1)
    {
      cout << "error using certif" << endl;
    }
  if (SSL_CTX_use_RSAPrivateKey_file(m_ssl_context, "C:\\home\\texane\\ziafs\\branches\\ziis\\conf\\ssl_certs\\privkey.pem", SSL_FILETYPE_PEM) != 1)
    {
      cout << "error using key" << endl;
    }

  cout << "ssl context got" << endl;
}

mod_ssl::~mod_ssl()
{
  ERR_free_strings();
  EVP_cleanup();
  SSL_CTX_free(m_ssl_context);

  cout << "deletingopensll" << endl;
}
