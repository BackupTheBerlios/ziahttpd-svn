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
#include <stdlib.h>
#include <time.h>


using namespace std;


class cgi
{
public:
  cgi();
  ~cgi();
  int  len_;
  void nl();
  string path_img(int type);
};

cgi::cgi()
{
  len_ = 0;
  cout << "<html><body>" << endl;
  cout << "<table border=0 cellpadding=0 cellspacing=0>" << endl;
}

cgi::~cgi()
{
  cout << "</table>" << endl;
  cout << "</body></html>" << endl;
}

void cgi::nl()
{
  cout << "<br/>" << endl;
}

string		cgi::path_img(int type)
{
	string	path = "";

	for (int i = 0; i < len_; i++)
		path = path + "../";
	if (type == 1)
		path = path + "./img/file.gif";
	else
		path = path + "./img/folder.gif";
	return (path);
}

static void make_title(char *path, cgi& c)
{
	for (int i = 0; path[i]; i++)
		if (path[i] == '/')
			c.len_++;
	c.len_ -= 3;
	cout << "<tr>" << endl;
	cout << "<td colspan=6><font size=5><b>Index of file:" << path << "</b></font></td>" << endl;
	cout << "</tr>" << endl;
	cout << "<tr>" << endl;
	cout << "<td colspan=6>&nbsp;</td>" << endl;
	cout << "</tr>" << endl;
}


// directory listing

static void report_path(const char* path, struct stat& st, cgi& c)
{
  struct tm *t;
  DIR* d;

  t = localtime(&st.st_ctime);
  cout << "<tr>" << endl;
  if (!(d = opendir(path))) // FILE
  {
    cout << "<td><img src = \"" << c.path_img(1) << "\">" << "&nbsp;</td>" << endl;
	cout << "<td><a href = \"" << path << "\">" << path << "</a>&nbsp;</td>" << endl;
	cout << "<td>" << st.st_size / 1024 << "&nbsp;</td>" << endl;
	cout << "<td>KO&nbsp;</td>" << endl;
  }
  else // FOLDER
  {
    closedir(d);
    cout << "<td><img src = \"" << c.path_img(2)<< "\">" << "&nbsp;</td>" << endl;
	cout << "<td><a href = \"" << path << "\">" << path << "</a>&nbsp;</td>" << endl;
	cout << "<td>&nbsp;</td>" << endl;
	cout << "<td>&nbsp;</td>" << endl;
  }
  printf("<td>%02d/%02d/%02d&nbsp;</td>\n", t->tm_mday, t->tm_mon + 1, t->tm_year % 100);
  printf("<td>%02d:%02d:%02d&nbsp;</td>\n", t->tm_hour, t->tm_min, t->tm_sec);
  cout << "</tr>" << endl;
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

  if (ac == 1)
  {
    make_title(".///", c);
    list_directory(".///", c);
  }
  else
  {
    make_title(av[1], c);
    list_directory(av[1], c);
  }
}
