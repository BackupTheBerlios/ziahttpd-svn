#ifndef ZIAFS_HTTP_HH
# define ZIAFS_HTTP_HH

#include <vector>
#include <map>
#include <ziafs_utils.hh>
#include <ziafs_buffer.hh>
#include <ziafs_config.hh>
#include <ziafs_status.hh>

namespace resource { class handle; class manager; }

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
  };

  class uri
  {
  public:
		uri();
		status::error					extension(std::string& localname);
    std::string&					localname() { return m_localname; };
    std::string&					wwwname() { return m_wwwname; };
    int&									status_code() { return m_status_code; };
		bool									reset();

  private:
    std::string	m_wwwname;
    std::string	m_localname;
    int					m_status_code;
  };


  class http : public protocol
  {
  public:
		// session management
		typedef enum
		{
			CHUNK_FIRST = 0,
			CHUNK_MIDDLE,
			CHUNK_LAST
		} chunk_pos_t;
    http();

    bool									consume(unsigned char *, unsigned int , bool&);
		bool									consume_body(buffer& dest, buffer* source);
    status::error					produce(buffer&);
    status::error					dump(buffer&);
    std::string						method() { return m_method; };
		bool									reset();
		bool									request_header_validation();
		unsigned int					body_size();
		bool									create_resource(resource::handle*&, resource::manager&, config&);

		// Response
		bool									create_header(buffer&, size_t, chunk_pos_t);
		bool									modify_header(config&, chunk_pos_t);
		bool									stringify_header(buffer&);

	private:
		bool	valid_method();
		bool	valid_uri();
		bool	valid_version();
		bool	valid_host();
		bool	method_can_have_body();
		//Response
		enum resouce_type_t
		{
			IS_FILE,
			IS_CGI,
			EXEC_BY_CGI,
			IS_FLY
		};
		bool									stringify_status_line(std::string&);
		bool									generate_header_lines(size_t, bool);
		bool									generate_content_type(config&);
		bool									generate_header_date();
		bool									generate_content_length(size_t);
		bool									error_code_string(std::string&);
		bool									get_type_of_resource(net::config&, resouce_type_t&);
		bool									pre_create_resource(net::config&);
		class data_enco
    {
    public:
      data_enco() { m_done = false; };
      virtual status::error	encode(buffer&, size_t, chunk_pos_t) = 0;
      virtual status::error	decode(net::protocol*, utils::line&, buffer&) = 0;
      buffer&	buff() { return m_buf; };
      bool		done();
      virtual ~data_enco() {}
    protected:
      buffer	m_buf;
      bool		m_done;
    };
    class chunked : public data_enco
    {
    public:
      chunked() : data_enco() { m_state = HDRLINE; };
      status::error	encode(buffer&, size_t, chunk_pos_t);
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
      unchunked() { m_state = FIRSTTIME; m_size_read = 0; };
      status::error	encode(buffer&, size_t, chunk_pos_t) { ziafs_return_status(status::SUCCESS); };
      status::error	decode(net::protocol*, utils::line&, buffer&);
    private:
      enum e_state
			{
				FIRSTTIME = 0,
				OTHERTIME
			};
      size_t			m_size;
      e_state			m_state;
			size_t			m_size_read;
    };

    status::error	parse_status_line(std::string&);
    status::error	parse_header_line(std::string&);
    status::error	handle_metadata();

    utils::line													m_line;
    enum e_state
      {
				STUSLINES = 0,
				HDRLINES,
				BODYDATA
      };
		class			http_data
		{
		public:
			std::string& operator[](const std::string&);
			std::string& operator=(const std::string&);
			std::string	m_version;
			std::map<std::string, std::string>	m_hdrlines;
			data_enco		*m_data_enco;
			bool				is_chunk;
		};
		std::string	m_query;
		e_state			m_state;
    uri					m_uri;
		std::string	m_method;

	public:
		http_data							request;
		http_data							response;
  };
}

#endif
