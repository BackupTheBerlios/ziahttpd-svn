#ifndef MOD_PSP_HH
# define MOD_PSP_HH


#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>
#include <sys/sysapi.hh>
#include <ziis.hh>

#ifdef _WIN32
# include <windows.h>
# include <sys/pthread.h>
# include "include/buffer.hh"
# include "perl/perl.h"
#else
# include <pthread.h>
# include "buffer.hh"
# include <EXTERN.h>
# include <perl.h>
#endif // _WIN32

#include <sys/sysapi.hh>



class mod_psp : public IModule, public IDocumentGenerator
{
public:
	mod_psp();
	~mod_psp() {};

	// implement IModule
	bool ReadConfig(const char*);
	const char* GetModuleName();
	const char* GetModuleVersion();
	void OnLoad(IFS*);

	// implement IDocumentGenerator
	void				GenerateDocument(IInput&, const char*, IOutput&);
	const char** GetSupportedMime() { return (const char **)mimes; }
private:
	char **mimes;
	void				GeneratePerl(buffer& out, buffer& in);
	//net::config *conf;
};


#endif // ! MOD_PSP_HH
