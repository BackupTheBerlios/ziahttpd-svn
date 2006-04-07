#include "include/mod_psp.hh"

using namespace std;

IModule* GetNewInstance()
{
  return new mod_psp;
}

static bool split_token(string& str_input, std::vector<string>& dest, bool putdel)
{
  char* buf = (char*)str_input.c_str();
  char* tmp = (char*)buf;
  char*	prev = tmp;

  for (int i = 0; i < (int)str_input.size() - 1; i++, tmp++)
    {
      if ((!strncmp(tmp, "<%", 2)) || (!strncmp(tmp, "%>", 2)))
	{
	  char t = *tmp;
	  *tmp = '\0';

	  string btmp(prev, strlen(prev));
	  dest.push_back(btmp);

	  *tmp = t;
	  if (putdel)
	    {
	      btmp.clear();
	      char f[3];

	      f[0] = *tmp;
	      f[1] = *(tmp + 1);
	      f[2] = '\0';
	      btmp = f;
	      dest.push_back(btmp);
	    }
	  tmp += 2;
	  prev = tmp;
	}
    }
  dest.push_back(string(prev, strlen(prev)));
  return (true);
}

void	mod_psp::GeneratePerl(string& out, string& in)
{
	PerlInterpreter *my_perl;
	char *script;
	char *embedding[] = { "", "-e", "0" };
	std::string	pre_script;
	SV* SV_ret;

	script = (char*)in.c_str();
	if ((my_perl = perl_alloc()) == NULL)
	{
		out = "Perl Alloc error";
		return ;
	}

	perl_construct( my_perl );
	perl_parse(my_perl, NULL, 3, embedding, NULL);
	perl_run(my_perl);
	SV_ret = eval_pv((const char *)script, TRUE);
	perl_destruct(my_perl);
	perl_free(my_perl);
	std::ostringstream filename;

	filename << "psp_";
#ifdef _WIN32
	filename  << (unsigned int)pthread_self().p;
#else
	filename  << (unsigned int)pthread_self();
#endif
	std::ifstream			iff(filename.str().c_str());
	std::ostringstream	off;

	off << iff.rdbuf();
	out = off.str();
}

void mod_psp::GenerateDocument(IInput& inp, const char* localname, IOutput& out)
{
  std::vector<string> block;
  std::vector<string>::iterator it;
  std::ifstream iff(localname);
  std::ostringstream off;
  std::ostringstream pre_script;

  if (!sysapi::file::is_path_valid(localname))
    {
      out.SetStatusCode(404);
      out.SendError(404);
      return ;
    }
  if (!sysapi::file::is_readable(localname))
    {
      out.SetStatusCode(403);
      out.SendError(403);
      return ;
    }

#ifdef _WIN32
  off << "<% open(FOO, \">psp_" << (unsigned int)pthread_self().p <<  "\") || die; $handle = select(FOO); %>" << iff.rdbuf() ;
#else
  off << "<% open(FOO, \">psp_" << (unsigned int)pthread_self() <<  "\") || die; $handle = select(FOO); %>" << iff.rdbuf() ;
#endif // _WIN32

  string str_input(off.str());
  str_input += " <% select ($handle); close (FOO); %>";
  string bout("\r\nprint <<endmarker;\r\n");
  int state = 0;

  split_token(str_input, block, true);

  for (it = block.begin(); it != block.end(); it++)
    {
      const char *str = it->c_str(); 
      if ((str[0] == '<') && (str[1] == '%'))
	{
	  bout += "\r\nendmarker\r\n";
	  state = 1;
	}
	
      else if ((str[0] == '%') && (str[1] == '>'))
	{
	  bout += "\r\nprint <<endmarker;\r\n";
	  state = 0;
	}
      else
	{
	  bout += (*it);
	}
    }
  if (!state)
    bout += "\r\nendmarker\r\n";

  string bin;
  std::ostringstream oss;

  GeneratePerl(bin, bout);

  // remove our temporary file
#ifdef _WIN32
  oss << "psp_" << (unsigned int)pthread_self().p;
#else
  oss << "psp_" << (unsigned int)pthread_self();
#endif //_WIN32
  sysapi::file::remove(oss.str());

  char size[20];
  sprintf(size, "%i", bin.size());
  out.SetOutput("Content-Length", (const char *)size);
  out.SetStatusCode(200);
  out.SendHeader();
  out.SendBuffer(bin.c_str(), (int)bin.size());
}
