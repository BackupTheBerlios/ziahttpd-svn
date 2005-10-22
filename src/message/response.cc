#include <message.hh>
#include <session.hh>
#include <sysapi.hh>
#include <iostream>

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


	sprintf(sizes, "%l", size);
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
	std::string	DOCROOT(".");
	std::string	err_str;
	std::string statusl;

	file_ = DOCROOT + page_;
	session_->http_info_.is_file_ = true;
	if (sysapi::file::is_directory(file_.c_str()))
	{
		if (!check_default_type(file_))
		{
			// ask for execute the cgi list_directory
			file_ = DOCROOT + "/error.html";
			session_->http_info_.is_file_ = false;
			session_->http_info_.is_cgi_ = true;
			std::cout << "Execute cgi list_directory" << std::endl;
		}
	}
	if (!sysapi::file::is_readable(file_.c_str()))
		error_code_ = 401;
	if (!sysapi::file::exists(file_.c_str()))
		error_code_ = 404;
	if (!error_code_string(err_str, file_))
	{
		error_code_ = 400;
		error_code_string(err_str, file_);
	}
	char err[4];
	sprintf(err, "%i", error_code_);
	response_statusline_ = "HTTP/" + version_ + " " + err + " " + err_str + "\r\n";
	session_->http_info_.filename_ = new char[file_.size() + 1];
	strcpy(session_->http_info_.filename_, file_.c_str());
	session_->http_info_.is_file_ = true;
	return (true);
}

bool		http::message::make_header()
{
	response_header_content_length();
	response_header_content_type();
	response_header_server();
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
		std::cout << "seek " << DEF[i] << std::endl;
		if (sysapi::file::is_readable(tmp.c_str()))
		{
			std::cout << "find " << DEF[i] << std::endl;
			dest = tmp;
			return (true);
		}
	}
	return (false);
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

	//sysapi::file::size(file_.c_str(), &size);
	//std::string	tmp;
	size = session_->http_info_.sz_body_;
	sprintf(sizestr, "Content-Length: %i", (int)size);
	response_header_.push_back(sizestr);
	return (true);
}

bool		http::message::response_header_server()
{
	response_header_.push_back("Server: KeKette HTTPD beta 0.1");
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
