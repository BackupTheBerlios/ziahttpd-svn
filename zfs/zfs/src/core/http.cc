#include <core/ziafs_net.hh>
#include <core/zia_stringmanager.hh>
#include <core/ziafs_debug.hh>
#include <core/ziafs_io.hh>

net::http::http()
{
	process_stage_fn = http::first_stage;
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

status::error	net::http::consume(net::session *s, buffer &buf)
{
	std::string	ln;
	if (m_line.from_buffer(ln, buf) == true)
	{
		ziafs_debug_msg("line : %s\n", ln.c_str());
	}
	//if (state != BODY)
	//{
	//	// if readline
	//		// parsing
	//		// 

	//} else {
	//	// readdata
	//}

	ziafs_return_status(status::NOTIMPL);
}

status::error	net::http::produce(buffer &buf)
{
	ziafs_return_status(status::NOTIMPL);
}
