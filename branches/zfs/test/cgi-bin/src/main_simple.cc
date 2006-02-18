//
// main_simple.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Feb 18 14:51:00 2006 texane
// Last update Sat Feb 18 14:56:59 2006 texane
//



#include <iostream>


using namespace std;


class cgi
{
public:
  cgi();
  ~cgi();
  void nl();
};

cgi::cgi()
{
  cout << "<html><body>" << endl;
}

cgi::~cgi()
{
  cout << "</body></html>" << endl;
}

void cgi::nl()
{
  cout << "<br/>" << endl;
}


extern char** environ;

int main(int ac, char** av)
{
  cgi c;
  int n;

  cout << "hello from the cgi" << endl;

  // display argv
  c.nl();
  cout << "** argv **" << endl;
  c.nl();
  for (n = 0; n < ac; ++n)
    {
      cout << "[" << n << "]" << av[n] << endl;
      c.nl();
    }

  // display environ
  c.nl();
  cout << "** envn **" << endl;
  for (n = 0; environ[n]; ++n)
    {
      cout << "[" << n << "]" << environ[n] << endl;
      c.nl();
    }

}
