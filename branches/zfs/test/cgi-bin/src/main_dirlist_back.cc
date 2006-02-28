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

static void report_path(const char* path, struct stat& st, cgi& c)
{
  cout << "<a href=" << path << ">" << path << "</a>";
  c.nl();
}


static void list_directory_rec(char* path, cgi& c)
{
  struct stat st;
  DIR* d;
  struct dirent* de;
  char full_path[1024];
  char* curr;

  strcpy(full_path, path);
  d = opendir(path);
  if (d)
    {
      curr = full_path + strlen(full_path);
      curr[0] = '/';
      curr[1] = 0;
      curr += 1;
      while ((de = readdir(d)))
	{
	  if (stat(full_path, &st) != -1)
	    {
	      strcpy(curr, de->d_name);
	      report_path(de->d_name, st, c);
	    }
	}
      closedir(d);
    }
}

static void list_directory(char* path, cgi& c)
{
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
