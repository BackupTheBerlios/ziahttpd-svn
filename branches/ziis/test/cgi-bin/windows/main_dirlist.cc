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
#include <string>
#include <iostream>


using namespace std;


class cgi
{
public:
  cgi();
  ~cgi();
  int		len_;
  void		nl();
  string	path_img(int);
};

cgi::cgi()
{
  len_ = 0;
  cout << "" << endl; 
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

static void report_path(char* path, WIN32_FIND_DATA& infos, cgi& c)
{
  SYSTEMTIME				t;
  WIN32_FILE_ATTRIBUTE_DATA attr;

  cout << "<tr>" << endl;
  if (!(infos.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) // FICHIER
  {
	cout << "<td><img src = \"" << c.path_img(1) << "\">" << "&nbsp;</td>" << endl;
	cout << "<td><a href = \"" << path << "\">" << path << "</a>&nbsp;</td>" << endl;
	if (GetFileAttributesEx(path, GetFileExInfoStandard, &attr))
	{
		cout << "<td>" << attr.nFileSizeLow / 1000 << "&nbsp;</td>" << endl;
		cout << "<td>KB" << "&nbsp;</td>" << endl;
	}
	else
	{
		cout << "<td>&nbsp;</td>" << endl;
		cout << "<td>&nbsp;</td>" << endl;
	}
  }
  else // DOSSIER
  {
	cout << "<td><img src = \"" << c.path_img(2)<< "\">" << "&nbsp;</td>" << endl;
	cout << "<td><a href = \"" << path << "\">" << path << "</a>&nbsp;</td>" << endl;
	cout << "<td>&nbsp;</td>" << endl;
	cout << "<td>&nbsp;</td>" << endl;
  }
  

  // SIZE
  
  if (FileTimeToSystemTime(&infos.ftLastWriteTime, &t))
  {
	cout << "<td>" << t.wMonth << "/" << t.wDay << "/" << t.wYear << "&nbsp;</td>" << endl;
	cout << "<td>" << t.wHour << ":" << t.wMinute << ":" << t.wSecond << "&nbsp;</td>" << endl;
  }
  else
  {
	cout << "<td>&nbsp;</td>" << endl;
	cout << "<td>&nbsp;</td>" << endl;
  }
  cout << "</tr>" << endl;
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

  if (ac == 1)
    {
	  make_title(".///", c);
      dir_list(".", c);
    }
  else
    {
	  make_title(av[1], c);
      dir_list(av[1], c);
    }
}
