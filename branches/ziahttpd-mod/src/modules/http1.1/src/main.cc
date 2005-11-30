//
// main.cc for  in /home/texane/texane/wip/ept3/zia/ziahttpd/src/modules/test
// 
// Made by 
// Login   <texane@epita.fr>
// 
// Started on  Sun Nov 13 21:01:23 2005 
// Last update Wed Nov 30 16:21:18 2005 texane
//


#include <zia.hh>
#include <string>
#include <iostream>
#include <dataman/stringmanager.hh>
#include <time.h>

using std::cout;
using std::cerr;
using std::endl;
using std::string;
bool	error_code_string(int status, std::string &dest);

bool	response_header_date(string& dest)
{
	char					*datestr;
	stringmanager::string	p;
	std::vector<std::string> v;
	time_t t;
	struct tm *tb;

	t = time(NULL);
	tb = localtime(&t);
	datestr = ctime(&t);
	datestr[strlen(datestr) - 1] = '\0';
	p.split(datestr, " ", v);
	dest = v[0] + ", " + v[2] + " " 
		+ v[1] + " " + v[4] + " " + v[3] + " GMT";
	return (true);
}


// List of exported functions
MOD_EXPORT( HK_ALTER_RESP_METADATA )(http::session&, server::core*, int&);
MOD_EXPORT( HK_PARSE_RQST_METADATA )(http::session& session, server::core* core, int& status);

// Exported function definitions


MOD_EXPORT(HK_PARSE_RQST_METADATA) (http::session& session, server::core* core, int& status)
{
	std::list<dataman::buffer>& hdrlines = session.hdrlines_in();
	std::list<dataman::buffer>::iterator ite;

	ite = hdrlines.begin();
	if (!session.info_in().parse_rqstline_statusline(*ite, session.uri()))
		session.uri().status() = 400;
	ite++;
	for( ; ite != hdrlines.end(); ite++)
	{
//		cout << (*ite).c_str() << endl;
		if (!session.info_in().parse_rqstline_headerline(*ite, session.uri()))
			session.uri().status() = 400;
	}
	cout << "version : " << session.info_in().version_string() << endl;
	cout << "uri : " << session.info_in().uri_string() << endl;
	cout << "method : " << session.info_in().method_string() << endl;
	cout << "query : " << session.info_in().query_string() << endl;
	session.info_out() = session.info_in();
	session.uri().widename() = session.info_in().uri_string();
	return (true);
}

MOD_EXPORT(HK_BUILD_RESP_METADATA) (http::session& session, server::core* core, int& status)
{

	session.info_out().build_respline(session.info_in(), session.uri());
	session.info_out()["Serveur"] = "TrancheD Ejambon 0.1 beta version ;)";
	session.info_out()["Connection"] = "close";
	response_header_date(session.info_out()["Date"]);

	return (true);
}


MOD_EXPORT(HK_ALTER_RESP_METADATA) (http::session& session, server::core* core, int& status)
{
	error_code_string(session.uri().status(), session.uri().strstatus());
	if (session.chunked())
	{
		session.info_out()["content-length"] = "";
		session.info_out()["Transfer-Encoding"] = "chunked";
	}
	if (session.first_chunk())
	{
		session.info_out().stringify_respline(session.hdrlines_out(), session.uri());
		session.info_out().reset();
		session.first_chunk() = false;
	} else {
		session.hdrlines_out().clear();
	}
	//session.hdrlines_out().display();
	return (true);
}

MOD_EXPORT(HK_ALTER_RESP_DATA) (http::session& session, server::core* core, int& status)
{

 cerr<<"ENTERING THE ALTERRESPDATA"<<endl;

	if (session.chunked() && session.last_chunk() == false)
	{
		string					hex;
		stringmanager::string	p;
		dataman::buffer			tmp;

		  cerr <<"ENTERING CH"<<endl;

		p.dec_to_hex(session.content_out().size(), hex);
		tmp = session.content_out();
		session.content_out() = hex + "\r\n";
		session.content_out() += tmp;
		//cout << session.content_out().size() << "\n" << session.content_out().c_str() << endl;
	  }
	else if (session.last_chunk() == true)
	  {
	    // Thisis thelast onetobesent
	    cout <<".>>>>>>>>sendingthelastchunk!"<<endl;
	    session.chunked() = false;
	    session.hdrlines_out().clear();
	    session.content_out() = "0";
	    session.content_out() += "\r\n";
	  }

	return (true);
}
