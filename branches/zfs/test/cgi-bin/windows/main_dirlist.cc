//
// main_dirlist.cc for  in 
// 
// Made by texane
// Login   <texane@gmail.com>
// 
// Started on  Wed Feb 22 00:55:36 2006 texane
// Last update Wed Feb 22 02:04:09 2006 texane
//



#include <windows.h>
#include <string.h>
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


static void report_path(char* path, WIN32_FIND_DATA& infos, cgi& c)
{
  cout << "<a href = \"" << path << "\">" << path << "</a>";
  c.nl();
}

static void dir_list_rec(char* path, cgi& c)
{
  char meta_path[1024];
  WIN32_FIND_DATA find_data;
  HANDLE srch_handle;
  bool done;

  strcpy(meta_path, path);
  strcat(meta_path, "\\*");

  done = false;
  srch_handle = FindFirstFile(meta_path, &find_data);
  if (srch_handle != INVALID_HANDLE_VALUE)
    {
      do
	{
	  report_path(find_data.cFileName, find_data, c);
	}
      while (FindNextFile(srch_handle, &find_data) == TRUE);
      FindClose(srch_handle);
    }
  else
    {
      cout << "Cannot browse directory";
      c.nl();
    }
}

static void dir_list(char* path, cgi& c)
{
  dir_list_rec(path, c);
}


extern char** environ;

int main(int ac, char** av)
{
  cgi c;

  cout << "directory listing";
  c.nl();
  if (ac == 1)
    {
      dir_list(".", c);
    }
  else
    {
      dir_list(av[1], c);
    }
}
