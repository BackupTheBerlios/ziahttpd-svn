//
// log.hh for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Nov 22 03:29:51 2005 texane
// Last update Wed Nov 23 11:02:18 2005 texane
//


#ifndef DEBUG_HH
# define DEBUG_HH


// ?
// Log functionnalities.
// Simple class providing a way to
// output cleanly on a stream. Component
// are compile time pluggable, modifiable,
// so that no effort are required to modify
// or improve it.
// The primary goal is to provide a way for
// developpers to see what they want among
// a flaw of information just by modifying a
// little bit the stream behaviour (manipulators)


#include <iomanip>
#include <iostream>
#include <sstream>
#include <ostream>


namespace debug
{
  enum
    {
# if defined (ENABLE_DEBUG_LOGGING)
      logging_enabled = 1
# else
      logging_enabled = 0
# endif // ENABLE_DEBUG_LOGGING
    };

  // ?
  // Identity of the logged resource is
  // implemented through the applicator
  // or stream modifier facility.

  typedef enum
    {
      MODULE = 0,
      THREAD,
      SERVER
    } identity_t;

  template<identity_t ID>
  std::ostream& identity(std::ostream& os)
  { return os; }

  template<>
  std::ostream& identity<MODULE>(std::ostream& os)
  { return os << "module['" __FILE__ "']"; }

  template<>
  std::ostream& identity<THREAD>(std::ostream& os)
  { return os << "thread[0x00000000]"; }



  // ?
  // Puncutation modifier.
  // The default punctuation symbol
  // is set to '>'.
  class setpunct
  {
  public:
    setpunct(char punct)
    { setpunct::punct_ = punct; }
   
    friend
    std::ostream& operator<<(std::ostream& os, setpunct& punct)
    { return os; }

    static char punct_;
  };

  // ?
  // Indentation modifier.
  // By default, indentation is set
  // to be empty(0 spaces).
  class setindent
  {
  public:
    setindent(int nindent)
    { setindent::nindent_ = nindent; }
   
    friend
    std::ostream& operator<<(std::ostream& os, setindent& indent)
    { return os; }

    static int nindent_;
  };

  int setindent::nindent_ = 0;
  char setpunct::punct_ = '>';

  

  // ?
  // Formatting manipulators.
  // Plug them together, for instance
  // newline is indent + marge.

  typedef enum
    {
      NEWLINE = 0,
      INDENT,
      TITLE,
      BORDER
    } modfmt_t;

  template<modfmt_t ID>
  std::ostream& fmt(std::ostream& os)
  { return os; }

  template<>
  std::ostream& fmt<INDENT>(std::ostream& os)
  {
    std::ostringstream strm;
    for (int i = 0; i < setindent::nindent_; ++i)
      strm << ' ';
    return os << strm.str();
  }

  template<>
  std::ostream& fmt<BORDER>(std::ostream& os)
  { return os << setpunct::punct_ << ' '; }

  template<>
  std::ostream& fmt<NEWLINE>(std::ostream& os)
  { return os << "\n" << fmt<INDENT> << fmt<BORDER>; }

  template<>
  std::ostream& fmt<TITLE>(std::ostream& os)
  { return os << fmt<INDENT> << "@"; }

  
  // Helper macros...
# define debug_open(ID) if (debug::logging_enabled) { std::cout << debug::fmt<debug::TITLE> << debug::identity<debug::ID> << ":" << debug::fmt<debug::NEWLINE>
# define debug_close()  << std::endl; }

  
}


#endif // ! DEBUG_HH
