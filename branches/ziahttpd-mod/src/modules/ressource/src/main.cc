//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Mon Nov 14 16:19:53 2005 
//


#include <zia.hh>
#include <string>
#include <iostream>
#include <vector>
#include <dataman/stringmanager.hh>

using std::cout;
using std::cerr;
using std::endl;
using std::string;



enum type_t {
	ISCGI = 0,
	ISFILE,
	ISRAW
}  ;

struct info_t {
	std::string			content_type;
	type_t				type;
	std::vector<string>	binary;
};

bool	have_directoryindex(http::session& session);
bool	check_typemine(http::uri &uri, info_t &info);

// List of exported functions
MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session& session, server::core*, int&);


// Exported function definitions
MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session& session, server::core* core, int&)
{
	info_t info;
if ((session.uri().localname()[session.uri().localname().size() - 1] == '/')
	&& (!have_directoryindex(session)))
	{
	//	create ressouce cgi for directory listing
		return (true);
	}
	printf("type mine\n");
	check_typemine(session.uri(), info);
	session.info_out()["content-type"] = info.content_type;
	return true;
}

bool	have_directoryindex(http::session& session)
{
	string	str[] = {"index.html", "index.htm", ""};
	string	tmp;

	for (int i = 0; !str[i].empty(); i++)
	{
		tmp = session.uri().localname() + str[i];
		printf("index : %s\n",tmp.c_str());
		if (sysapi::file::is_readable(tmp.c_str()))
		{
			session.uri().localname() = tmp;
			session.uri().widename() = session.uri().widename() + str[i];
			printf("directory index found : %s\n", session.uri().localname().c_str());
			return (true);
		}
	}
	printf("directory index not found\n");
	return (false);
}


bool check_typemine(http::uri &uri, info_t &info)
{
	stringmanager::string	p;
	struct LALA_s {
		char		*str;
		char		*type;
		bool		cgi;
		char		*path;
	};
	LALA_s	LALA[] = {
		{"text/html", "html", false, ""},
		{"text/html", "htm", false, ""},
		{"image/gif", "gif", false, ""},
		{"image/png", "png", false, ""},
		{"text/html", "exe", true, "/usr/bin/php -i"},
		{0, 0, 0, 0}
	};
	uri.build_extension();
	for (int i = 0; LALA[i].str; i++)
	{
			printf("type mine %s\n", LALA[i].str);
		if (uri.extension() == LALA[i].type)
		{
			if (LALA[i].cgi)
			{
				info.type = ISCGI;
				p.split(LALA[i].path, " ", info.binary);
			}
			else
				info.type = ISFILE;
			info.content_type = LALA[i].str;
			break ;
		}
	}
	info.content_type = "text/html";
	info.type = ISFILE;
	return (false);
}
