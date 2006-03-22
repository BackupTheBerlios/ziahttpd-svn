//
// main_loop.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Tue Feb 21 15:17:00 2006 texane
// Last update Tue Feb 21 15:17:53 2006 texane
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
  while (1)
    {
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
}
