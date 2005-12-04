//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Sun Dec 04 18:30:48 2005 texane
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


#define CHUNKSZ  1024



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
//MOD_EXPORT( HK_GET_RQST_DATA )(http::session& session, server::core*, int&);
MOD_EXPORT( HK_BUILD_RESP_DATA )(http::session& session, server::core*, int&);

bool	check_cgi(http::uri& uri, const string& file, info_t& info)
{
	stringmanager::string	p;

	
	p.split(file, " ", (vector<string>&)info.binary);
	  // cout << "FILE : " << info.binary[0] << endl;
	if (!sysapi::file::exists(info.binary[0].c_str()))
	{
	  // cout << "CGI DOESN T EXIST" << endl;
		uri.status() = 404;
		return (false);
	}
	if (!sysapi::file::is_executable(info.binary[0].c_str()))
	{
	  // cout << "CGI CANN T BE EXECUTE" << endl;
		uri.status() = 401;
		return (false);
	}
	info.content_type = "text/html";
	info.type = ISCGI;
	  // cout << "LE CGI EST VALIDE" << endl;
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
		{404, "http://www.barbie.com", "", "", "fd"},
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
//					fo pas ce foutre dela gueule du monde
//					admin rezo apprend a config le server
					return (true);
				}
				if (!sysapi::file::exists(LALA[i].file))
				{
					session.uri().status() = 404;
//					fo pas ce foutre dela gueule du monde
//					admin rezo apprend a config le server
					return (true);
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
				info.type = ISRAW;
				return (true);
			}
		}
	}
	return (true);
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
						+ "/" + session.services_->query_conf_simple(session, "cgidirlist"),
						info))
			{
				//info.binary.push_back(session.services_->query_conf_simple(session, "cgiroot") + session.services_->query_conf_simple(session, "cgidirlist"));
				//cout << "LALAL :" << session.uri().localname() << endl;
				info.binary.push_back(session.uri().localname());
			}
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


	if (session.info_in().body() == true)
	{
		dataman::buffer tmp;
		int				size;			


		session.services_->create_resource(session, session.uri().localname());
		session.resource()->open(dataman::resource::O_FEEDONLY, err);
		size = atoi(session.info_in()["Content-length"].c_str());
		session.services_->create_resource_in(session, session.hsock_con(), size);
		  session.resource_in()->open(dataman::resource::O_FETCHONLY, err);
		if (session.resource_in()->fetch(tmp, err) != dataman::resource::EOFETCHING)
		  {
		    cout <<  tmp.to_string() << endl;
			session.resource()->feed(tmp, err);
			  }
		session.resource()->close(err);
		session.resource_in()->close(err);
		return (true);
	}

	info.type = UNSET;
	if (!session.chunked())
	{
		fill_info(session, info);
		check_typemine(session.uri(), info);
		session.info_out()["content-type"] = info.content_type;
		if (info.type == ISFILE)
			session.services_->create_resource(session, session.uri().localname());
		if (info.type == ISRAW)
			session.services_->create_resource(session, session.uri().status());
		if (info.type == ISCGI)
		{
		  // cout << "c'est CGI" << endl;
			vector<const string> env;
			//cout << info.binary[0] << " : " << info.binary[1] << endl;
			session.services_->create_resource(session,
						       (const vector<const string>)info.binary,
						       (const vector<const string>)env);

		}
		if (info.type != ISNONE
			&& info.type != UNSET)
		{
			//check type method
			if (!session.resource()->open(dataman::resource::O_FETCHONLY, err))
				printf("status code internal error\n");
			else
			    session.first_chunk() = true;
		}
	} else {
		info.type = ISRAW;
	}
	if (info.type != ISNONE
		&& info.type != UNSET)
	{

		// if method have body
		// resourcein fetch in tmp buffer
		// resource feed the tmp buffer
		// else
		// if method doesn't have body
		if (!session.resource()->fetch(session.content_out(), CHUNKSZ, err))
		  cout << "[!]FETCH TROUBLE" << endl;
		else
		{
			//cout << "ERR:" << err << endl;
			//cout << "FETCH SIZE :" <<  session.content_out().size() << endl;
			if (err == dataman::resource::ESUCCESS)
			{
				session.chunked() = true;
				//std::cout << "IS CHUNKED" << std::endl;
			}
			else if (err == dataman::resource::EOFETCHING)
			{
				session.chunked() = true; 
				session.last_chunk() = true; 
				session.resource()->close(err);
				//std::cout << "END OF  CHUNKED" << std::endl;
			}
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
		{"image/bmp", "bmp", false, ""},
		{"image/png", "png", false, ""},
 		{"text/html", "exe", true, ""},
 		{"text/plain", "wmv", false, ""},
		{0, 0, 0, 0}
	};
	if (info.type == ISRAW)
		return (true);
	uri.build_extension();
	for (int i = 0; LALA[i].str; i++)
	{
		printf("type mine %s\n", LALA[i].str);
		if (uri.extension() == LALA[i].type)
		{
			if (LALA[i].cgi)
			{
			  // std::cout << "C EST UN CGI" << std::endl;
				//sysapi::file::normalize_name(uri.localname());
				check_cgi(uri, uri.localname(), info);
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
