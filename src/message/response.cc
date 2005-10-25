#include <message.hh>
#include <session.hh>
#include <sysapi.hh>
#include <iostream>
#include <stringmanager.hh>
#include <time.h>

bool		http::message::make_response()
{
	make_statusline();
	make_header();
	make_body();

	return (true);
}

bool		http::message::stringify()
{
	session_->http_info_.buf_statusline_ = new char[response_statusline_.size() + 1];
	strcpy(session_->http_info_.buf_statusline_, response_statusline_.c_str());
	build_header_for_send();
	return (true);
}

bool		http::message::bodysize(unsigned long size)
{
	char	sizes[100];


	sprintf(sizes, "%d", (int)size);
	std::list<std::string>::iterator theIterator;
	for(theIterator = response_header_.begin(); theIterator != response_header_.end(); theIterator++)
	{
		std::string::size_type pos = theIterator->find("Content-Length:", 0);
		if (pos != std::string::npos)
		{
			std::string tmp("Content-Length: ");
			*theIterator = tmp + sizes;
			std::cout << "->size : " << *theIterator << std::endl;
		}
	}

	return (true);
}

bool		http::message::make_statusline()
{
//	std::string	err_str;
	std::string statusl;

	file_ = http::DOCROOT + page_;
	std::cout << "file :" << file_ << std::endl;
	session_->http_info_.is_file_ = true;
	if (sysapi::file::is_directory(file_.c_str()))
	{
		std::cout << "file is  a directory" << std::endl;
		if (!check_default_type(file_))
		{
			// ask for execute the cgi list_directory
			file_ = /*http::DOCROOT + */CGIDIRLIST;
			session_->http_info_.is_file_ = false;
			session_->http_info_.is_cgi_ = true;
			std::cout << "Execute cgi list_directory" << std::endl;
			make_statusline_hi();
			return (true);
		}
	}
	if (!sysapi::file::is_readable(file_.c_str()))
		error_code_ = 401;
	if (!sysapi::file::exists(file_.c_str()))
		error_code_ = 404;
	make_statusline_hi();
	session_->http_info_.is_file_ = true;
	return (true);
}

void		http::message::make_statusline_hi()
{
	char err[4];
	std::string err_str;

	if (!error_code_string(err_str, file_))
	{
		error_code_ = 400;
		std::cout << "CANN'T UNDERSTAND STATUS LINE" << std::endl;
		error_code_string(err_str, file_);
	}
	sprintf(err, "%i", error_code_);
	response_statusline_ = "HTTP/" + version_ + " " + err + " " + err_str + "\r\n";
	session_->http_info_.filename_ = new char[file_.size() + 1];
	strcpy(session_->http_info_.filename_, file_.c_str());
}
bool		http::message::make_header()
{
	response_header_content_length();
	response_header_content_type();
	response_header_server();
	response_header_date();
	return (true);
}

bool		http::message::make_body()
{

	return (true);
}

bool		http::message::check_default_type(std::string &dest)
{
	const char *DEF[] = {"index.html", "index.htm", 0};
	std::string	tmp;
	int	i;

	std::cout << "--" <<dest << std::endl;
	for (i = 0; DEF[i]; i++)
	{
		tmp = dest + DEF[i];
		std::cout << "seek " << DEF[i] << "source: " << tmp << std::endl;
		if (sysapi::file::exists(tmp.c_str()))
		{
			std::cout << "find " << DEF[i] << std::endl;
			dest = tmp;
			return (true);
		}
	}

	// errocode 301
	for (i = 0; DEF[i]; i++)
	{
		tmp = dest + "/" + DEF[i];
		std::cout << "seek " << DEF[i] << "source: " << tmp << std::endl;
		if (sysapi::file::exists(tmp.c_str()))
		{
			std::string		ret("Location: ");
			std::cout << "find " << DEF[i] << std::endl;
			dest = tmp;
			error_code_ = 301;
			ret += "http://" + header_["host"] + page_ + "/";
//			ret += "http://www.barbie.com";
			std::cout << "redirection: " << ret << std::endl;
			response_header_.push_back(ret);
			return (true);
		}
	}
	return (false);
}

bool	http::message::response_header_date()
{
	struct tm				*newtime;
	time_t					aclock;
	char					*datestr;
	stringmanager::string	p;
	std::vector<std::string> v;
	std::string				tmp;

	time(&aclock);
	newtime = localtime(&aclock);
	datestr = asctime(newtime);
	datestr[strlen(datestr) - 1] = '\0';
	p.split(datestr, " ", v);
	tmp = "Date: " + v[0] + ", " + v[2] + " " 
		+ v[1] + " " + v[4] + " " + v[3] + " GMT";
	response_header_.push_back(tmp);
	//free (datestr);
	return (true);
}

bool	http::message::response_header_content_type()
{
	struct LALA_s {
		char		*str;
		char		*type;
		bool		cgi;
	};
	LALA_s	LALA[] = {
		{"text/html", "html", false},
		{"text/html", "htm", false},
		{"image/gif", "gif", false},
		{"text/html", "exe", true},
		{0, 0}
	};
	std::string			type;

	std::cout << "file " << file_ << std::endl;
	std::string::size_type	pos = file_.rfind(".", file_.size() - 1);
	type = file_.substr(pos + 1, file_.size() - pos);
//	std::cout << "type: " << type << " pos:" << pos << std::endl;
	for (int i = 0; LALA[i].str; i++)
	{
		if (LALA[i].type == type)
		{
			char		  tmp[1000];

			if (LALA[i].cgi)
			{
				session_->http_info_.is_cgi_ = true;
				session_->http_info_.is_file_ = false;
			}
			sprintf(tmp, "Content-Type: %s", LALA[i].str);
			response_header_.push_back(tmp);
			return (true);
		}
	}
	response_header_.push_back("Content-Type: text/html");
	return (true);
}

bool	http::message::response_header_content_length()
{
	unsigned long size = 0;
	char		  sizestr[1000];

	size = session_->http_info_.sz_body_;
	sprintf(sizestr, "Content-Length: %i", (int)size);
	response_header_.push_back(sizestr);
	return (true);
}

bool		http::message::response_header_server()
{
	response_header_.push_back("Server: TrancheD Ejambon Ver : sec");
	return (true);
}

bool	http::message::build_header_for_send()
{
	size_t	size = 0;

	std::list<std::string>::iterator theIterator;
	for(theIterator = response_header_.begin(); theIterator != response_header_.end(); theIterator++)
		size += theIterator->size() + 2;
	size += 2;
	session_->http_info_.buf_headerlines_ = new char[size + 1];
	memset(session_->http_info_.buf_headerlines_, 0, size + 1);
	for(theIterator = response_header_.begin(); theIterator != response_header_.end(); theIterator++ )
	{
		strcat(session_->http_info_.buf_headerlines_, theIterator->c_str());
		strcat(session_->http_info_.buf_headerlines_, "\r\n");
	}
	strcat(session_->http_info_.buf_headerlines_, "\r\n");
	return (true);
}
