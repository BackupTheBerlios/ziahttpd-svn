#include <fstream>
#include <sstream>
#include "include/mod_psp.hh"
#include <vector>

#ifdef _WIN32
# include <windows.h>
# include <sys/pthread.h>
#else
# include <pthread.h>
#endif
#include <perl.h>
#include <sys/sysapi.hh>
//#include <semaphore.h>
//#include <scheduler.h>

IModule* GetNewInstance()
{
	return new mod_psp;
}


bool	split_token(buffer& source, std::vector<buffer>& dest, bool putdel)
{
	char* buf = source.c_str();
	char* tmp = (char*)buf;
	char*	prev = tmp;

	int state = 0;

	for (int i = 0; i < (int)source.size(); i++, tmp++)
	{
		if ((!strncmp(tmp, "<%", 2)) || (!strncmp(tmp, "%>", 2)))
		{
			char t = *tmp;

			*tmp = '\0';
			buffer	btmp((const unsigned char*)prev, strlen(prev));
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
			tmp +=2;
			prev = tmp;
		}
	}
	buffer	btmp((const unsigned char*)prev, strlen(prev));
	dest.push_back(btmp);
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
	delete script;
	std::ostringstream filename;

	filename << "psp_";
	filename  << (unsigned int)pthread_self().p;
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
		buffer b((const unsigned char*)off.str().c_str(), off.str().size());
		b += " <% select ($handle); close (FOO); %>";
		buffer bout;
		int state = 0;

		split_token(b, block, true);
		bout += "\r\nprint <<endmarker;\r\n";
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
		}
		if (!state)
			bout += "\r\nendmarker\r\n";

	buffer bin;
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
	char *send_buf = bin.c_str();
	out.SendBuffer(send_buf, (int)bin.size());
	delete send_buf;
}

/*print <<endmarker;
line1
line2
line3
etc.
endmarker*/
