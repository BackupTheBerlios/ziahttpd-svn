//
// main_simple.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Feb 18 14:51:00 2006 texane
// Last update Sat Feb 18 15:02:36 2006 texane
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
  string s;

  cout << "hello from the cgi" << endl;
  c.nl();

  while (cin >> s)
    {
      cout << "<from_input>: " << s << endl;
      c.nl();
    }

  cout << "end of cgi" << endl;
  c.nl();
}