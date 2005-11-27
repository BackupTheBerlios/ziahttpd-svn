//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Thu Nov 24 19:55:53 2005 texane
//


#include <zia.hh>
#include <string>
#include <iostream>
#include <vector>
#include <dataman/stringmanager.hh>
#include <dataman/buffer.hh>
#include <dataman/resource.hh>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using dataman::buffer;
using dataman::resource;



enum type_t {
	UNSET = 0,
	ISCGI,
	ISFILE,
	ISRAW,
	ISNONE
}  ;

struct info_t {
	std::string			content_type;
	type_t				type;
	std::vector<const string>	binary;
	std::string			new_localtion;
};

bool	have_directoryindex(http::session& session);
bool	check_typemine(http::uri &uri, info_t &info);

// List of exported functions
MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session& session, server::core*, int&);

bool	check_cgi(http::uri& uri, const string& file, info_t& info)
{
	stringmanager::string	p;

	p.split(file, " ", (vector<string>&)info.binary);
	if (!sysapi::file::exists(info.binary[0].c_str()))
	{
		uri.status() = 404;
		return (false);
	}
	if (!sysapi::file::is_executable(info.binary[0].c_str()))
	{
		uri.status() = 401;
		return (false);
	}
	info.content_type = "text/html";
	info.type = ISCGI;
	return (true);
}

bool	check_status_code(http::session& session, info_t& info)
{
	struct LALA_s {
		int			status;
		char		*location;
		char		*file;
		char		*newstatus;
		char		*buffer;
	};
	//pu this in config file 
	LALA_s	LALA[] = {
		{404, "", "", "", "ssss"},
		{401, "", "", "", "lala c l'erreur de merde"},
		{-1, 0, 0, 0, 0}
	};
	int				i;

	for(i = 0; LALA[i].status != -1; i++)
	{
		if (session.uri().status() == LALA[i].status)
		{
			if (*LALA[i].file)
			{
				if (!sysapi::file::is_readable(LALA[i].file))
				{
					session.uri().status() = 401;
					return (false);
				}
				if (!sysapi::file::exists(LALA[i].file))
				{
					session.uri().status() = 404;
					return (false);
				}
				session.uri().localname() = LALA[i].file;
				info.type = ISFILE;
				return (true);
			}
			if (*LALA[i].newstatus)
			{
				session.uri().status() = atoi(LALA[i].newstatus);
				// for the moment only full uri works ...
				session.info_out()["Location"] = LALA[i].location;
				info.type = ISNONE;
				return (false);
			}
			if (*LALA[i].buffer)
			{
				std::cout<< "c'est du RAW " << std::endl;
				info.type = ISRAW;
				return (true);
			}
		}
	}
}

bool	fill_info(http::session& session, info_t& info)
{
	// check if it's a directory index
	if (session.uri().localname()[session.uri().localname().size() - 1] == '/')
	{
		if (!have_directoryindex(session))
		{
			if (check_cgi(session.uri(),
						session.services_->query_conf_simple(session, "cgiroot")
						+ session.services_->query_conf_simple(session, "cgidirlist"),
						info))
			return (true);
		}
	}
	if (!sysapi::file::is_readable(session.uri().localname().c_str()))
		session.uri().status() = 401;
	if (!sysapi::file::exists(session.uri().localname().c_str()))
		session.uri().status() = 404;
	while (!check_status_code(session, info))
		;
}


// Exported function definitions
MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session& session, server::core* core, int&)
{
	info_t info;
	dataman::resource::error_t err;
	char	size[20];
	bool	err_code = true;

	info.type = UNSET;
	fill_info(session, info);
	check_typemine(session.uri(), info);

	session.info_out()["content-type"] = info.content_type;
	if (info.type == ISFILE)
	{
		session.services_->create_resource(session, session.uri().localname());
	}
	if (info.type == ISRAW)
	{
		cout << "FDFDSSDFDFDSFSD" << endl;
		session.services_->create_resource(session, session.uri().status());
	}

	if (info.type == ISCGI)
	  {
	    //resource::error_t err;
	    //vector<const string> av;
	    vector<const string> env;
	   // av.push_back("../root/www/cgi-get-windows.exe");
	    session.services_->create_resource(session,
					       (const vector<const string>)info.binary,
					       (const vector<const string>)env);
	  }
	  if (!session.resource()->open(dataman::resource::O_FETCHONLY, err))
	  {
		  //status code internal error
		  printf("status code internal error\n");
	  }
	  session.resource()->fetch(session.content_out(), err);
	  session.resource()->close(err);
	  //get the size of the buffer for add the content length entry to the response header
	sprintf(size, "%d", session.content_out().size());
	session.info_out()["content-length"] = size;
	return true;
}

bool	have_directoryindex(http::session& session)
{
	string	tmp;
	std::vector<std::string>::iterator iter;
	std::vector<std::string>& vectmp = session.services_->query_conf_complex(session, "directoryindex");

	for(iter = vectmp.begin();
		iter != vectmp.end(); iter++)
	{
		tmp = session.uri().localname() + *iter;
		printf("index : %s\n",tmp.c_str());
		if (sysapi::file::is_readable(tmp.c_str()))
		{
			session.uri().localname() = tmp;
			session.uri().widename() = session.uri().widename() + *iter;
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
 		{"text/html", "exe", true, "../root/www/dir_list_windows.exe"},
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
				check_cgi(uri, LALA[i].path, info);
				return (false);
			}
			else
			{
				info.type = ISFILE;
				info.content_type = LALA[i].str;
				return (true);
			}
		}
	}
	info.content_type = "text/html";
	if (info.type == UNSET)
 		info.type = ISFILE;
	return (true);
}
