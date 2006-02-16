#ifndef ZIAFS_HTTP_HH
# define ZIAFS_HTTP_HH

#include <vector>
#include <map>
#include <ziafs_utils.hh>
#include <ziafs_buffer.hh>
#include <ziafs_config.hh>
#include <ziafs_status.hh>


namespace net
{
  class protocol
  {
  public:
    virtual ~protocol() {}

    // protocol interface
    virtual bool consume(unsigned char *, unsigned int , bool&) = 0;
    virtual status::error	produce(buffer&) = 0;
    virtual status::error	dump(buffer&) = 0;
    // switch function pointer
//    status::error (*process_stage_fn)(session*);
    
  };

  class uri
  {
  public:
    static status::error	extension(std::string& localname);
    std::string&		localname() { return m_localname; };
    std::string&		wwwname() { return m_wwwname; };
    int&			status_code() { return m_status_code; };
		bool			reset();
  private:
    std::string	m_wwwname;
    std::string	m_localname;
    int	m_status_code;
  };


  class http : public protocol
  {
  public:
    http();
    std::string& operator[](const std::string&);
    std::string& operator=(const std::string&);
    bool									consume(unsigned char *, unsigned int , bool&);
    status::error					produce(buffer&);
//    static status::error	first_stage(session*);
//    static status::error	second_stage(session*);
//    static status::error	third_stage(session*);
    status::error					dump(buffer&);
    std::string						method() { return m_method; };
		bool									reset();
  private:

    class data_enco
    {
    public:
      data_enco() { m_done = false; };
      virtual status::error	encode(buffer&) = 0;
      virtual status::error	decode(net::protocol*, utils::line&, buffer&) = 0;
      buffer&	buff() { return m_buf; };
      bool	done();
      virtual ~data_enco() {}
    protected:
      buffer	m_buf;
      bool		m_done;
    };
    class chunked : public data_enco
    {
    public:
      chunked() : data_enco() { m_state = HDRLINE; };
      status::error	encode(buffer&) { ziafs_return_status(status::SUCCESS); }
      status::error	decode(net::protocol*, utils::line&, buffer&);
    private:
      enum e_state
	{
	  HDRLINE = 0,
	  BODYDATA
	};
    private:
      int					m_chunk_size;
      e_state			m_state;
    };
    class unchunked : public data_enco
    {
    public:
      unchunked() { m_state = FIRSTTIME; };
      status::error	encode(buffer&) { ziafs_return_status(status::SUCCESS); };
      status::error	decode(net::protocol*, utils::line&, buffer&);
    private:
      enum e_state
	{
	  FIRSTTIME = 0,
	  OTHERTIME
	};
      int					m_size;
      e_state			m_state;

    };

    status::error	parse_status_line(std::string&);
    status::error	parse_header_line(std::string&);
    status::error	handle_metadata();

    std::map<std::string, std::string>	m_hdrlines;
    utils::line													m_line;
    enum e_state
      {
				STUSLINES = 0,
				HDRLINES,
				BODYDATA
      };
    e_state			m_state;
    std::string	m_method;
    std::string	m_version;
    std::string	m_query;
    uri					m_uri;
    data_enco		*m_data_enco_req;
		data_enco		*m_data_enco_res;
  public:
//    data_enco*	dataenco(){ return m_data_enco; };
  };

}

#endif