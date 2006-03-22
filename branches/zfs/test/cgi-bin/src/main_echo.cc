//
// main_simple.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Feb 18 14:51:00 2006 texane
// Last update Thu Feb 23 02:24:48 2006 texane
//



#include <string>
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
  int nret;
  unsigned char buffer[4000];
  int nread;

  cout << "hello from the cgi" << endl;
  c.nl();

//   while (cin >> s)
  nread = 0;
  while (nread < 2000)
    {
      nret = _read(0, (unsigned char*)buffer + nread, 200);
      if (nret <= 0)
	{
	  strcpy((char*)buffer, "cannot read");
	}
      else
	{
	  buffer[nread + nret] = 0;
	  nread += nret;
	}
    }
  cout << "from_input: " << buffer << endl;
  c.nl();
  cout << "end of cgi" << endl;
  c.nl();
}
