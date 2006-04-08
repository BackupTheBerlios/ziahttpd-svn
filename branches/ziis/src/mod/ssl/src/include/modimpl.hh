//
// modimpl.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Apr 01 10:28:21 2006 texane
// Last update Sat Apr 08 20:28:19 2006 texane
//


#ifndef MODIMPL_HH
# define MODIMPL_HH


#ifdef _WIN32
# include "openssl/ssl.h"
# include "openssl/err.h"
#else
# include <openssl/ssl.h>
# include <openssl/err.h>
#endif // _WIN32

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <sys/sysapi.hh>
#include <ziis.hh>

# include "config.hh"
# include "string_manager.hh"


# define SSL_CERT	"conf/ssl_certs/cacert.pem"
# define SSL_PRIVKEY	"conf/ssl_certs/privkey.pem"

class mod_ssl : public IModule, public IConnection
{
public:
  typedef struct
  {
    BIO* m_bio;
    SSL* m_ssl;
  } _ssl_data_t;

public:
  // ctor/dtor
  mod_ssl(const std::string& = "conf/ziafs.xml");
  ~mod_ssl();

  // implement IModule
  bool ReadConfig(const char*);
  const char* GetModuleName();
  const char* GetModuleVersion();
  void OnLoad(IFS*);

  // implement IConnection
  short GetPort();
  const char* GetHost();
  void* Accept(SOCKET);
  int Recv(SOCKET, void*, char*, int);
  int Send(SOCKET, void*, const char*, int);
  void Close(SOCKET, void*);


private:
  // connection
  std::string m_host;
  short m_port;

  // ssl related
  SSL_METHOD* m_ssl_method;
  SSL_CTX* m_ssl_context;

  // module are resetable
  void reset();
  void release();
  bool reload(const std::string&);

  // config
  net::config* m_config;
  std::string m_cert_file;
  std::string m_privkey_file;
};


#endif //MODIMPL_HH
