#include <message.hh>
#include <session.hh>
#include <sysapi.hh>
#include <iostream>

bool		http::message::make_response()
{
	make_statusline(&session_->http_info_.buf_statusline_);
	std::cout << "SEND : " <<session_->http_info_.buf_statusline_ << std::endl;
	make_header(session_->http_info_.buf_headerlines_);
	make_body(session_->http_info_.buf_body_, session_->http_info_.sz_response_body_);
	return (true);
}

bool		http::message::make_statusline(char** dest)
{
	std::string	DOCROOT(".");
	std::string	err_str;
	std::string statusl;
	char		*tmp;

	file_ = DOCROOT + page_;
	if (sysapi::file::is_directory(file_.c_str()))
	{
		if (!check_default_type(file_))
		{
			// ask for execute the cgi list_directory
			std::cout << "Execute cgi list_directory" << std::endl;
		}
	}
	if (!sysapi::file::is_readable(file_.c_str()))
		error_code_ = 401;
	if (!sysapi::file::exists(file_.c_str()))
		error_code_ = 404;
	if (!error_code_string(err_str))
	{
		error_code_ = 400;
		error_code_string(err_str);
	}
	char err[4];
	sprintf(err, "%i", error_code_);
	statusl = "HTTP/" + version_ + " " + err + " " + err_str + "\r\n";
	tmp = new char[statusl.size()];
	strcpy(tmp, statusl.c_str());
	*dest = tmp;
	return (true);
}

bool		http::message::make_header(char* dest)
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
			std::string	tmp;

			//tmp = "Content-Type: " + LALA[i].str;
			header_.push_back(tmp);
			return (true);
		}
	}

	header_.push_back("Content-Type: text/html");

	return (true);
}

bool		http::message::make_body(unsigned char *dest, sysapi::socket_in::size_t& size)
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

