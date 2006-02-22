//
// main_simple.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Sat Feb 18 14:51:00 2006 texane
// Last update Wed Feb 22 00:47:47 2006 
//



#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>


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



// directory listing

static char g_path[1024];

static void report_path(const char* path, struct stat& st, cgi& c)
{
  cout << "<a href=" << g_path << ">" << path << "</a>";
  c.nl();
}


static void list_directory_rec(char* path, cgi& c)
{
  struct stat st;
  DIR* d;
  struct dirent* de;
  char* curr;

  d = opendir(path);
  if (d)
    {
      curr = path + strlen(path);
      curr[0] = '/';
      curr[1] = 0;
      curr += 2;
      while ((de = readdir(d)))
	{
	  if (stat(g_path, &st) != -1)
	    {
	      strcpy(curr, de->d_name);
	      report_path(curr, st, c);
	    }
	}
      closedir(d);
    }
}

static void list_directory(char* path, cgi& c)
{
  strcpy(g_path, path);
  list_directory_rec(path, c);
}


extern char** environ;

int main(int ac, char** av)
{
  cgi c;

  cout << "** listing directory **\n";
  c.nl();
  if (ac == 1)
    list_directory(".", c);
  else
    list_directory(av[1], c);
}
