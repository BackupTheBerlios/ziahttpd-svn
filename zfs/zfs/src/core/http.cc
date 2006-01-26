#include <core/ziafs_net.hh>
#include <core/zia_stringmanager.hh>
#include <core/ziafs_debug.hh>
#include <core/ziafs_io.hh>
#include <vector>
#include <sstream>

net::http::http()
{
	process_stage_fn = http::first_stage;
	m_state = STUSLINES;
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

status::error net::http::first_stage(session* s)
{
	buffer*	buf;

	if (s->m_client->io_on_read((void*&)buf) == status::SUCCESS)
	{
		if (buf)
		{
			s->m_proto->consume(s, *buf);
			// proto->consum(session, buffer);
			// session->target = new io::res...
			// cout << buf->tostring() << endl;
			delete buf;
		}
	}
	ziafs_return_status(status::SUCCESS);
}

status::error net::http::second_stage(session* s)
{
	buffer*	buf;
	if (s->m_client->io_on_read((void*&)buf) == status::SUCCESS)
	{
		if (buf)
		{
			s->m_proto->consume(s, *buf);
			delete buf;
		}
	}
	ziafs_return_status(status::NOTIMPL);
}

status::error	net::http::consume(net::session *s, buffer &buf)
{
	std::string	ln;

	if (m_state == BODYDATA)
	{
		m_data_enco->decode(m_line, buf);
		ziafs_return_status(status::SUCCESS);
	}
	if (m_line.from_buffer(ln, buf) == true)
	{
		ziafs_debug_msg("line : %s\n", ln.c_str());
		if (ln.empty())
		{
			process_stage_fn = http::second_stage;
			ziafs_debug_msg("end of header, go to second stage ;)%s\n", "");
			handle_metadata();
			m_state = BODYDATA;
			ziafs_return_status(status::SUCCESS);
		}
		if (m_state == HDRLINES)
		{
			parse_header_line(ln);
			ziafs_return_status(status::SUCCESS);
		}
		if (m_state == STUSLINES)
		{
			parse_status_line(ln);
			m_state = HDRLINES;
			ziafs_return_status(status::SUCCESS);
		}
	}
	ziafs_return_status(status::SUCCESS);
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
		m_hdrlines[key] = val;
	}
	else
    ziafs_return_status(status::HEADERLINE_FAILED);
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

	if (m_hdrlines["transfer-encoding"] == "chunked")
		m_data_enco = new chunked;
	else
		m_data_enco = new unchunked;
	ziafs_return_status(status::SUCCESS);
}

status::error				net::http::chunked::decode(utils::line& m_line, buffer& buf)
{
	

	if (m_state == HDRLINE)
	{
		std::string		ln;
		if (m_line.from_buffer(ln, buf) == true)
		{
			stringmanager::hex_to_int((const std::string&)ln, m_chunk_size);
			ziafs_debug_msg("chunk size-> %i\n", m_chunk_size);
			m_state = BODYDATA;
		}
	}
	if (m_state == BODYDATA)
	{
//			m_buffer.from_buffer(buf);
		//if (m_chunk_size != buffer.size())
//				m_buf += buf;
//		if (m_chunk_size == buffer.size())
				

	}
	ziafs_return_status(status::SUCCESS);
}
status::error	net::http::data_enco::done()
{
	if (m_done)
	{
		ziafs_return_status(status::SUCCESS);
	}
	else
	{
		ziafs_return_status(status::FAILED);
	}
}