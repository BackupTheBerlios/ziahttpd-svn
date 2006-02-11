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
	m_data_enco = NULL;
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

	if (s->m_server->res_manager()->fetch(s->m_client, (void*&)buf) == status::SUCCESS)
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

	ziafs_debug_msg("entering in stage 2%s\n", "");
	if (((net::http*)s->m_proto)->method() == "GET")
	{
		ziafs_debug_msg("No need for this method :%s, walk to the third stage\n",((net::http*)s->m_proto)->method().c_str());
		s->m_proto->process_stage_fn = http::third_stage;
		ziafs_return_status(status::NOTIMPL);
	}
	if (s->m_server->res_manager()->fetch(s->m_client, (void*&)buf) == status::SUCCESS)
	{
		if (buf)
		{
			s->m_proto->consume(s, *buf);
			delete buf;
		}
	}
	ziafs_return_status(status::NOTIMPL);
}

status::error net::http::third_stage(session* s)
{
	ziafs_debug_msg("entering in stage 3 wow%s\n", "");
	ziafs_return_status(status::NOTIMPL);
}

status::error	net::http::consume(net::session *s, buffer &buf)
{
	std::string	ln;

	if (m_state == BODYDATA)
	{
		if (m_data_enco->decode(s, m_line, buf) == status::ENDOFREQUEST)
		{
			s->m_proto->process_stage_fn = http::third_stage;
			ziafs_return_status(status::ENDOFREQUEST);
		}
		if (m_data_enco->done() == true)
		{
			delete m_data_enco;
			m_data_enco = NULL;
			handle_metadata();
		}
		ziafs_return_status(status::SUCCESS);
	}
	while (m_line.from_buffer(ln, buf) == true)
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
			if (parse_header_line(ln) == status::HEADERLINE_FAILED)
			{
				ziafs_debug_msg("HEADER LINE INCORRECT%s\n", "");
			}
			//ziafs_return_status(status::SUCCESS);
		}
		if (m_state == STUSLINES)
		{
			if (parse_status_line(ln) == status::STATUSLINE_FAILED)
			{
				ziafs_debug_msg("STATUS LINE INCORRECT%s\n", "");
			}
			m_state = HDRLINES;
			//ziafs_return_status(status::SUCCESS);
		}
		buf.reset();
		//ln.clear();
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
	{
		m_data_enco = new chunked;
		ziafs_return_status(status::SUCCESS);
	}
	if (atoi(m_hdrlines["content-length"].c_str()) > 0)
	{
		m_data_enco = new unchunked;
		ziafs_return_status(status::SUCCESS);
	}
	m_uri.status_code() = 411;
	ziafs_return_status(status::FAILED);
}

status::error				net::http::chunked::decode(net::session*, utils::line& m_line, buffer& buf)
{
	if (m_state == HDRLINE)
	{
		std::string		ln;

		if (m_line.from_buffer(ln, buf) == true)
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

status::error				net::http::unchunked::decode(net::session* s,utils::line& m_line, buffer& buf)
{
	if (m_state == FIRSTTIME)
	{
		m_line.get_bytes(m_buf);
		//const std::string t("lala");
		m_size = atoi((*(net::http*)s->m_proto)["content-length"].c_str()) + 2;
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
