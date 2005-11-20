//
// exception.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sun Nov 20 14:24:48 2005 texane
// Last update Sun Nov 20 14:47:21 2005 texane
//


#include <iostream>
#include <server/exception.hh>


using std::cout;
using std::cerr;
using std::endl;


// Errors enumeration
using server::exception::CONNECTION;
using server::exception::MEMORY;
using server::exception::UNKNOWN;


// Implementation of the CONNECTION
// related error handling class

bool	server::exception::error<CONNECTION>::report()
{
  cerr << "[*] Connection error reporting" << endl;
  return false;
}

bool	server::exception::error<CONNECTION>::solve()
{
  cerr << "[*] Connection error solving" << endl;
  return false;
}


// Implementation of the MEMORY
// allocation related errors

bool	server::exception::error<MEMORY>::report()
{
  return false;
}

bool	server::exception::error<MEMORY>::solve()
{
  return false;
}
