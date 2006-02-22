//
// main_simple.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Feb 18 14:51:00 2006 texane
// Last update Wed Feb 22 11:54:37 2006 texane
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
  unsigned char buffer[512];

  cout << "hello from the cgi" << endl;
  c.nl();

//   while (cin >> s)
  nret = read(0, buffer, sizeof(buffer));
  if (nret <= 0)
    {
      strcpy((char*)buffer, "cannot read");
    }
  else
    {
      buffer[nret] = 0;
    }
  cout << "from_input: " << buffer << endl;
  c.nl();
  cout << "end of cgi" << endl;
  c.nl();
}
