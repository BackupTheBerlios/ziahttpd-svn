#include "include/mod_psp.hh"
#include <iostream>

using namespace std;

IModule* GetNewInstance()
{
  return new mod_psp;
}


bool	split_token(buffer& source, std::vector<buffer>& dest, bool putdel)
{
  char* buf = source.c_str();
  char* tmp = (char*)buf;
  char*	prev = tmp;
  char* p_delete;

  p_delete = tmp;

  for (int i = 0; i < (int)source.size() - 1; i++, tmp++)
    {
      if ((!strncmp(tmp, "<%", 2)) || (!strncmp(tmp, "%>", 2)))
	{
	  char t = *tmp;

	  *tmp = '\0';

	  cout << "hehe" << endl;
	  cout << "entering: (" << strlen(prev) << ")" << endl;
	  cout << "passed" << endl;

	  buffer btmp((const unsigned char*)prev, strlen(prev));

	  cout << "entering with :: " << prev << endl;
	  cout << "length is     :: " << strlen(prev) << endl;
	  cout << "tostring      :: " << btmp.tostring() << endl;

	  cout << "after buffer" << endl;

	  dest.push_back(btmp);

	  cout << "after push_back" << endl;

	  *tmp = t;
	  if (putdel)
	    {
	      btmp.clear();
	      char f[3];

	      f[0] = *tmp;
	      f[1] = *(tmp + 1);
	      f[2] = '\0';
// 	      btmp = f;
// 	      dest.push_back(btmp);
	    }
	  tmp += 2;
	  prev = tmp + 1;
	}
    }
  buffer	btmp((const unsigned char*)prev, strlen(prev));
  dest.push_back(btmp);
  delete[] p_delete;
  return (true);
}

void	mod_psp::GeneratePerl(buffer& out, buffer& in)
{
	PerlInterpreter *my_perl;
	char *script;
	char *embedding[] = { "", "-e", "0" };
	std::string	pre_script;
	SV* SV_ret;

	script = in.c_str();
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
	delete[] script;
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

//void mod_psp::GenerateDocument(IInput& inp, const char* localname, IOutput& out)
//{
//	std::vector<buffer>						block;
//	std::vector<buffer>::iterator	it;
//	std::string										buf;
//	char													mbuf[4096];
//	buffer												bout;
//	int														state = 0;
//	int														fd;
//
//	//////////////////////////////////////////////////////////////////////////
//	//have to check is file exist
//	//////////////////////////////////////////////////////////////////////////
//
//	if ((fd = _open(localname, _O_RDONLY)) == NULL)
//	{
//		//////////////////////////////////////////////////////////////////////////
//		// senderror
//		// return 
//		//////////////////////////////////////////////////////////////////////////
//		
//	}
//	while (int i = _read(fd, mbuf, 4095))
//	{
//		mbuf[i] = '\0';
//		buf += mbuf;
//	}
//	_close(fd);
//	buffer b;
//	b = "<% sub zfs_print { $__bufret .= $_[0]; } %> ";
//	b += buf.c_str();
//	split_token(b, block, true);
//	bout += "\r\n$__bufret = '";
//	for (it = block.begin(); it != block.end(); it++)
//	{
//		buffer*	tmp = &(*it);
//		const	char *str = tmp->c_str(); 
//		if ((str[0] == '<') && (str[1] == '%'))
//		{
//			bout += "';\r\n";
//			state = 1;
//		}
//
//		else if ((str[0] == '%') && (str[1] == '>'))
//		{
//			bout += "\r\n$__bufret .='";
//			state = 0;
//		}
//		else
//		{
//			bout += (*it);
//		}
//	}
//	if (!state)
//		bout += "';\r\n";
//
//	buffer bin;
//	GeneratePerl(bin, bout);
//	//
//	//////////////////////////////////////////////////////////////////////////
//	
//	char size[20];
//	sprintf(size, "%i", bin.size());
//	out.SetOutput("Content-Length", (const char *)size);
//	out.SetStatusCode(200);
//	out.SendHeader();
//	char *send_buf = bin.c_str();
//	out.SendBuffer(send_buf, (int)bin.size());
//	delete send_buf;
//}

void mod_psp::GenerateDocument(IInput& inp, const char* localname, IOutput& out)
{
		std::vector<buffer>						block;
		std::vector<buffer>::iterator	it;
		std::ifstream			iff(localname);
		std::ostringstream	off;
		std::ostringstream	pre_script;

		cout << "entering generate document" << endl;

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

		cout << "open the dotpsp file" << endl;

#ifdef _WIN32
		off << "<% open(FOO, \">psp_" << (unsigned int)pthread_self().p <<  "\") || die; $handle = select(FOO); %>" << iff.rdbuf() ;
#else
		off << "<% open(FOO, \">psp_" << (unsigned int)pthread_self() <<  "\") || die; $handle = select(FOO); %>" << iff.rdbuf() ;
#endif // _WIN32

		buffer b((const unsigned char*)off.str().c_str(), off.str().size());
		b += " <% select ($handle); close (FOO); %>";
		buffer bout;
		int state = 0;

		cout << "splitting token" << endl;

		split_token(b, block, true);

		cout << "walkming token" << endl;


		bout = "\r\nprint <<endmarker;\r\n";
		for (it = block.begin(); it != block.end(); it++)
		{
			buffer*	tmp = &(*it);
			const	char *str = tmp->c_str(); 
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
			delete[] str;
		}
		if (!state)
		  bout += "\r\nendmarker\r\n";

	buffer bin;
	std::ostringstream oss;

	cout << "generate perl" << endl;

	GeneratePerl(bin, bout);

	cout << "generate perl done" << endl;
	
	// remove our temporary file
#ifdef _WIN32
	oss << "psp_" << (unsigned int)pthread_self().p;
#else
	oss << "psp_" << (unsigned int)pthread_self();
#endif //_WIN32
	sysapi::file::remove(oss.str());

	cout << "exiting generate document" << endl;

	char size[20];
	sprintf(size, "%i", bin.size());
	out.SetOutput("Content-Length", (const char *)size);
	out.SetStatusCode(200);
	out.SendHeader();
	char *send_buf = bin.c_str();
	out.SendBuffer(send_buf, (int)bin.size());
	delete[] send_buf;
}
