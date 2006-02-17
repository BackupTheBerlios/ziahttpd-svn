#include <ziafs_http.hh>
#include <ziafs_debug.hh>
#include <zia_stringmanager.hh>
#include <sstream>

net::http::http()
{
	m_state = STUSLINES;
	m_data_enco_req = NULL;
	m_data_enco_res = NULL;
}

bool				net::http::reset()
{
	ziafs_debug_msg("\nHTTP Reset %s\n", "");
	m_state = STUSLINES;
	m_method.clear();
	m_version.clear();
	m_query.clear();
	m_hdrlines.clear();
	if (m_data_enco_req)
		delete m_data_enco_req;
	if (m_data_enco_res)
		delete m_data_enco_res;
	m_data_enco_req = NULL;
	m_data_enco_res = NULL;
	m_line.reset();
	m_uri.reset();

	return true;

}

std::string&			net::http::operator[](const std::string& key)
{
	std::string str(key);

	stringmanager::normalize(str);
	return (m_hdrlines[str]);
}

std::string&			net::http::operator=(const std::string& val)
{
	std::string t = val;

	return ((std::string&)val);
}

bool	net::http::consume(unsigned char *data, unsigned int nbytes, bool &finished)
{
	buffer	buf(data, nbytes);
	std::string	ln;
	bool ln_toolong;


	finished = false;
	if (m_state == BODYDATA)
	{
		if (!m_data_enco_req)
			return false;
		if (m_data_enco_req->decode(this, m_line, buf) == status::ENDOFREQUEST)
		{
//			s->m_proto->process_stage_fn = http::third_stage;
			finished = true;
			return true;
		}
		if (m_data_enco_req->done() == true)
		{
			delete m_data_enco_req;
			m_data_enco_req = NULL;
			handle_metadata();
		}
		return true;
	}


	while (m_line.from_buffer(ln, buf, ln_toolong) == true)
	{
		ziafs_debug_msg("line : %s\n", ln.c_str());
		if (ln.empty() && (m_state != STUSLINES))
		{
//			process_stage_fn = http::second_stage;
			ziafs_debug_msg("end of header, go to second stage ;)%s\n", "");
			handle_metadata();
			m_state = BODYDATA;
//			process_stage_fn(s);
//		End of metadata
			finished = true;
			return true;
		}

		if (m_state == HDRLINES)
		{
			if (parse_header_line(ln) == status::HEADERLINE_FAILED)
			{
				ziafs_debug_msg("HEADER LINE INCORRECT%s\n", "");
				return false;
			}
			//ziafs_return_status(status::SUCCESS);
		}
		if (m_state == STUSLINES)
		{
			if (ln.empty())
				return (true);
			ziafs_debug_msg("line size ;)%i\n", ln.size());
			if (parse_status_line(ln) == status::STATUSLINE_FAILED)
			{
				ziafs_debug_msg("STATUS LINE INCORRECT%s\n", "");
				return false;
			}
			m_state = HDRLINES;
			//ziafs_return_status(status::SUCCESS);
		}
		buf.reset();
	}

	if (ln_toolong == true)
	  {
	    // Here do the necessary
	  }

	return true;
}

status::error					net::http::parse_status_line(std::string& ln)
{
	std::vector<std::string>	vec;

	stringmanager::split(ln, " ", vec);
	if (vec.size() != 3)
	{
		m_uri.status_code() = 400;
		ziafs_return_status(status::STATUSLINE_FAILED);
	}
	m_method = vec[0];
	
	size_t i = vec[1].find("?", 0);
	if (i != std::string::npos)
	{
		m_uri.wwwname() = vec[1].substr(0, i);
		m_query = vec[1].substr(i + 1, vec[1].length() - i - 1);
	}
	stringmanager::unconvert_hexa(m_uri.wwwname());
	stringmanager::unconvert_hexa(m_query);
	m_version = vec[2];

	m_uri.status_code() = 200;
	ziafs_return_status(status::STATUSLINE_SUCCESS);
}

status::error					net::http::parse_header_line(std::string& ln)
{
	std::string	key;
	std::string	val;

	size_t i = ln.find(":", 0);
	if (i != std::string::npos)
	{
		key = ln.substr(0, i);
		val = ln.substr(i + 1, ln.length() - i - 1);
		stringmanager::remove_space(val);
		stringmanager::normalize(key);
		if (key.empty())
		{
			m_uri.status_code() = 400;
			ziafs_return_status(status::HEADERLINE_FAILED);
		}
		m_hdrlines[key] = val;
	}
	else
	{
		m_uri.status_code() = 400;
		ziafs_return_status(status::HEADERLINE_FAILED);
	}
	ziafs_return_status(status::HEADERLINE_SUCCESS);
}

status::error					net::http::produce(buffer &buf)
{
	ziafs_return_status(status::NOTIMPL);
}

status::error					net::http::dump(buffer& buf)
{
	std::ostringstream stream;

	stream << "STATUS CODE :" << m_uri.status_code() << " WWWNAME :" << m_uri.wwwname() << " QUERY :" << m_query;
	buf = stream.str();
	ziafs_return_status(status::SUCCESS);
}
status::error					net::http::handle_metadata()
{
	if (m_data_enco_req)
	{
		ziafs_debug_msg("Ecncoding already defined ... %s\n", "");
		ziafs_return_status(status::FAILED);
	}
	if (m_hdrlines["transfer-encoding"] == "chunked")
	{
		m_data_enco_req = new chunked;
		ziafs_return_status(status::SUCCESS);
	}
	if (atoi(m_hdrlines["content-length"].c_str()) > 0)
	{
		m_data_enco_req = new unchunked;
		ziafs_return_status(status::SUCCESS);
	}
	m_uri.status_code() = 411;
	ziafs_return_status(status::FAILED);
}

status::error				net::http::chunked::decode(net::protocol*, utils::line& m_line, buffer& buf)
{
  bool ln_toolong;

	if (m_state == HDRLINE)
	{
		std::string		ln;

		if (m_line.from_buffer(ln, buf, ln_toolong) == true)
		{
			stringmanager::hex_to_int((const std::string&)ln, m_chunk_size);
			if (!m_chunk_size)
			{
				ziafs_return_status(status::ENDOFREQUEST);
			}
			m_chunk_size += 2; // \r\n

			ziafs_debug_msg("chunk size-> %i\n", m_chunk_size);
			m_line.get_bytes(m_buf);
			m_state = BODYDATA;
		}
		else if (ln_toolong == true)
		  {
		    // here do the necessary
		  }
	}
	if (m_state == BODYDATA)
	{
		m_buf += buf;
		if (m_chunk_size == (int)m_buf.size())
		{
			m_done = true;
			ziafs_debug_msg("FINI CHUNK %s", "");
//			std::cout << "SIZE :" << m_buf.size() << "\n" <<m_buf.tostring();
			// le buffer a 2 caractere en trop
		}
	}
	ziafs_return_status(status::SUCCESS);
}

status::error				net::http::unchunked::decode(net::protocol* s,utils::line& m_line, buffer& buf)
{
	if (m_state == FIRSTTIME)
	{
		m_line.get_bytes(m_buf);
		//const std::string t("lala");
		m_size = atoi((*(net::http*)s)["content-length"].c_str()) + 2;
		m_state = OTHERTIME;
	}
	m_buf += buf;
	if (m_size == (int)m_buf.size())
	{
		m_done = true;
//		std::cout << "SIZE :" << m_buf.size() << "\n" << m_buf.tostring();
		ziafs_return_status(status::ENDOFREQUEST);
	}
	ziafs_return_status(status::SUCCESS);
}

bool	net::http::data_enco::done()
{
	if (m_done)
	{
		return true;
	}
	else
	{
		return false;
	}
}
