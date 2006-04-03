#include "include/mod_psp.hh"
#include <vector>
#include "include/string_manager.hh"
#include <perl.h>
#include <iostream>
#include <stdio.h>

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
	SV* SV_ret;
	STRLEN n_a;
	
	script = in.c_str();
	my_perl = perl_alloc();
	perl_construct( my_perl );
	
	perl_parse(my_perl, NULL, 3, embedding, NULL);
	perl_run(my_perl);
	SV_ret = eval_pv((const char *)script, TRUE);
	out = SvPV(get_sv("__bufret", FALSE), n_a);
	perl_destruct(my_perl);
	perl_free(my_perl);
	delete script;
}

void mod_psp::GenerateDocument(IInput& inp, const char* localname, IOutput& out)
{
	std::vector<buffer>						block;
	std::vector<buffer>::iterator	it;
	std::string										buf;
	char													mbuf[4096];
	buffer												bout;
	int														state = 0;
	int														fd;

	//////////////////////////////////////////////////////////////////////////
	//have to check is file exist
	//////////////////////////////////////////////////////////////////////////
	
	if ((fd = _open(localname, _O_RDONLY)) == NULL)
	{
		//////////////////////////////////////////////////////////////////////////
		// senderror
		// return 
	}
	while (int i = _read(fd, mbuf, 4095))
	{
		mbuf[i] = '\0';
		buf += mbuf;
	}
	_close(fd);
	buffer b;
	b = "<% sub zfs_print { $__bufret .= $_[0]; } %> ";
	b += buf.c_str();
	split_token(b, block, true);
	bout += "\r\n$__bufret = '";
	for (it = block.begin(); it != block.end(); it++)
	{
		buffer*	tmp = &(*it);
		const	char *str = tmp->c_str(); 
		if ((str[0] == '<') && (str[1] == '%'))
		{
			bout += "';\r\n";
			state = 1;
		}

		else if ((str[0] == '%') && (str[1] == '>'))
		{
			bout += "\r\n$__bufret .='";
			state = 0;
		}
		else
		{
			bout += (*it);
		}
	}
	if (!state)
		bout += "';\r\n";

	buffer bin;
	GeneratePerl(bin, bout);
	//
	//////////////////////////////////////////////////////////////////////////
	
	char size[20];
	sprintf(size, "%i", bin.size());
	out.SetOutput("Content-Length", (const char *)size);
	out.SetStatusCode(200);
	out.SendHeader();
	char *send_buf = bin.c_str();
	out.SendBuffer(send_buf, (int)bin.size());
	delete send_buf;
}

//
//void mod_psp::GenerateDocument(IInput& inp, const char* localname, IOutput& out)
//{
//		std::vector<buffer>						block;
//		std::vector<buffer>::iterator	it;
//		const char toto[] = "<html> <% print \"fd\"; %> </html> <% $t = '$variable' %>";
//		buffer b((const unsigned char*)toto, strlen(toto));
//		buffer bout;
//		int state = 0;
//
//		split_token(b, block, true);
//		bout += "\r\nprint <<endmarker;\r\n";
//		for (it = block.begin(); it != block.end(); it++)
//		{
//			buffer*	tmp = &(*it);
//			const	char *str = tmp->c_str(); 
//			std::cout << str << std::endl;
//			//unsigned char b1 = tmp->operator[](0);
//			//unsigned char b2 = tmp->operator[](1);
//			if ((str[0] == '<') && (str[1] == '%'))
//			{
//				bout += "\r\nendmarker\r\n";
//				state = 1;
//			}
//	
//			else if ((str[0] == '%') && (str[1] == '>'))
//			{
//				bout += "\r\nprint <<endmarker;\r\n";
//				state = 0;
//			}
//			else
//			{
//				bout += (*it);
//			}
//		}
//		if (!state)
//			bout += "\r\nendmarker\r\n";
//		std::cout << bout.c_str();
//	
//	buffer bin;
//	GeneratePerl(bin, bout);
//	std::cout << bin.c_str();
//}

/*print <<endmarker;
line1
line2
line3
etc.
endmarker*/
