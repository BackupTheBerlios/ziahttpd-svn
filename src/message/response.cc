#include <message.hh>
#include <session.hh>
#include <sysapi.hh>
#include <iostream>

bool		http::message::make_response()
{
	make_statusline();
	make_header();
	make_body();

	session_->http_info_.buf_statusline_ = new char[statusline_.size() + 1];
	strcpy(session_->http_info_.buf_statusline_, statusline_.c_str());
//	std::cout << "SEND : " << session_->http_info_.buf_statusline_ << std::endl;
	build_header_for_send();
//	std::cout << "SEND : " << session_->http_info_.buf_headerlines_ << std::endl;

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
			file_ = DOCROOT + "error.html";
			session_->http_info_.is_file_ = false;
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
	statusline_ = "HTTP/" + version_ + " " + err + " " + err_str + "\r\n";
	session_->http_info_.filename_ = new char[file_.size() + 1];
	strcpy(session_->http_info_.filename_, file_.c_str());
	session_->http_info_.is_file_ = true;
	return (true);
}

bool		http::message::make_header()
{
	response_header_content_length();
	response_header_content_type();
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
	};
	LALA_s	LALA[] = {
		{"text/html", "html"},
		{"text/html", "htm"},
		{0, 0}
	};
	std::string			type;

	std::cout << "file " << file_ << std::endl;
	std::string::size_type	pos = file_.rfind(".", file_.size() - 1);
	type = file_.substr(pos + 1, file_.size() - pos);
	std::cout << "type: " << type << " pos:" << pos << std::endl;
	for (int i = 0; LALA[i].str; i++)
	{
		if (LALA[i].type == type)
		{
			char		  tmp[1000];

			sprintf(tmp, "Content-Type: %s", LALA[i].str);
			header_.push_back(tmp);
			return (true);
		}
	}
	header_.push_back("Content-Type: text/html");
	return (true);
}

bool	http::message::response_header_content_length()
{
	unsigned long size;
	char		  sizestr[1000];

	sysapi::file::size(file_.c_str(), &size);
	std::string	tmp;

	sprintf(sizestr, "Content-Length: %i", (int)size);
	header_.push_back(sizestr);
	return (true);
}

bool	http::message::build_header_for_send()
{
	int	size = 0;

	std::list<std::string>::iterator theIterator;
	for(theIterator = header_.begin(); theIterator != header_.end(); theIterator++)
		size += theIterator->size() + 2;
	size += 2;
	session_->http_info_.buf_headerlines_ = new char[size + 1];
	memset(session_->http_info_.buf_headerlines_, 0, size + 1);
	for(theIterator = header_.begin(); theIterator != header_.end(); theIterator++ )
	{
		strcat(session_->http_info_.buf_headerlines_, theIterator->c_str());
		strcat(session_->http_info_.buf_headerlines_, "\r\n");
	}
	strcat(session_->http_info_.buf_headerlines_, "\r\n");
	return (true);
}
