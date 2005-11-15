//
// cgi.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Oct 23 12:12:55 2005 
// Last update Sun Oct 23 14:20:46 2005 
//


#include <iostream>
#include <cstring>


using std::cout;
using std::endl;


static void test_post()
{
  int nbuf;
  int nread;
#define NBLK	200
  unsigned char blk[NBLK];
  unsigned char* buf;

  // Bufferise from stdin
  nbuf = 0;
  buf = 0;
  while ((nread = read(0, (void*)blk, NBLK)) > 0)
    {
      unsigned char* tmp;

      tmp = new unsigned char[nbuf + nread];
      if (buf)
	{
	  memcpy((void*)tmp, (const void*)buf, nbuf);
	  delete[] buf;
	}
      memcpy((void*)(tmp + nbuf), (const void*)blk, nread);
      buf = tmp;
      nbuf += nread;
    }

  for (int i = 0; i < nbuf; ++i)
    cout << (char)buf[i];
  cout << endl;
}


static void test_get(char** env)
{
  // Read from environ
  for (int i = 0; env[i]; ++i)
    cout << i << ": " << env[i] << "<br>";
  cout << endl;
}


int main(int, char**, char** env)
{
  cout << "<html>" << endl;
  cout << "<body>" << endl;
  cout << "This is a cgi generated html page." << endl;
  cout << "Displaying environment:" << endl;
  // test_get(env);
  test_post();
  cout << "</body>" << endl;
  cout << "</html>" << endl;
}
