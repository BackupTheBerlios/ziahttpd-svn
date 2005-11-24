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
	dataman::resource::error_t err;
	char	size[20];
	bool	err_code = false;

	if ((session.uri().localname()[session.uri().localname().size() - 1] == '/')
	&& (!have_directoryindex(session)))
	{
	//	create ressouce cgi for directory listing
		return (true);
	}
	check_typemine(session.uri(), info);
	session.info_out()["content-type"] = info.content_type;

	if (info.type == ISFILE)
	{
		if (!sysapi::file::is_readable(session.uri().localname().c_str()))
		{
			session.uri().status() = 401;
			err_code = true;
		}
		if (!sysapi::file::exists(session.uri().localname().c_str()))
		{
			session.uri().status() = 404;
			err_code = true;
		}

		if (!err_code)
			session.services_->create_resource(session, session.uri().localname());
		else
			session.services_->create_resource(session, session.uri().status());
		if (!session.resource()->open(err, dataman::resource::O_FETCHONLY))
		{
			//status code internal error
			printf("dans ton cul\n");
		}
		session.resource()->fetch(session.content_out(), err);
		session.resource()->close(err);
	}
	else if (info.type == ISCGI)
	  {
	    resource::error_t err;
	    vector<const string> av;
	    vector<const string> env;
	    av.push_back("../root/www/cgi-get-windows.exe");
	    session.services_->create_resource(session,
					       (const vector<const string>)av,
					       (const vector<const string>)env);
	    if (!session.resource()->open(err, dataman::resource::O_FETCHONLY))
	      {
		cout << "Cannot open resource" << endl;
	      }
	    else
	      {
		session.resource()->fetch(session.content_out(), 1000, err);
		session.resource()->close(err);
	      }

	  }
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
// 		{"text/html", "exe", true, "/usr/bin/php -i"},
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
				info.type = ISCGI;
				p.split(LALA[i].path, " ", info.binary);
				break;
			}
			else
				info.type = ISFILE;
			info.content_type = LALA[i].str;
			break ;
		}
	}
	info.content_type = "text/html";
// 	info.type = ISFILE;
	info.type = ISCGI;
	return (false);
}
