#ifdef WIN32
#include <fcntl.h>
#endif

#include "bnf.h"
#include "listen_sessionTcp.h"
#include "logsystem.h"

ListenSessionTcp::ListenSessionTcp( boost::asio::io_service& io_service, int read_wait_time_sec, int write_wait_time_sec )
	: ListenSession(io_service, SessionBase::LISTEN_SESSION)
	, acceptor_( io_service )
	, read_wait_time_sec_( read_wait_time_sec )
	, write_wait_time_sec_( write_wait_time_sec )
{
	bnf::instance()->tcp_listen_session_seq_.pop(handle_);
}

ListenSessionTcp::~ListenSessionTcp()
{
	bnf::instance()->tcp_listen_session_seq_.push(handle_);
}

bool ListenSessionTcp::Run( std::string& host, const int port, rnSocketIOHandler* func,
							size_t receive_buffer_size, size_t send_buffer_size )
{
	std::ostringstream oss;
	oss << port;

	std::string port_string = oss.str();

	boost::asio::ip::tcp::resolver resolver( io_service_ );

	boost::asio::ip::tcp::resolver::query query( host, port_string );
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );

	boost::system::error_code error;

	acceptor_.open( endpoint.protocol(), error );
	if( error )
	{
		LOG_ERROR( "bnf - acceptor_.open error. error[%s] / host[%s] / port[%d]", error.message().c_str(), host.c_str(), port);
		return false;
	}

	acceptor_.set_option( boost::asio::ip::tcp::acceptor::reuse_address( true ), error );
	if( error )
	{
		LOG_ERROR( "bnf - acceptor_.set_option error. error[%s] / host[%s] / port[%d]", error.message().c_str(), host.c_str(), port);
		return false;
	}

	acceptor_.bind( endpoint, error );
	if( error )
	{
		LOG_ERROR( "bnf - acceptor_.bind error. error[%s] / host[%s] / port[%d]", error.message().c_str(), host.c_str(), port);
		return false;
	}

	acceptor_.listen( boost::asio::socket_base::max_connections, error );
	if( error )
	{
		LOG_ERROR( "bnf - acceptor_.listen error. error[%s] / host[%s] / port[%d]", error.message().c_str(), host.c_str(), port);
		return false;
	}

	closeOnExecOn();

	SetUserData(func);

	session_handle shandle;
	if( bnf::instance()->tcp_session_seq_.pop(shandle) == false )
	{
		bnf::instance()->growSessionBuffer();
		LOG_INFO("session buffer growing.");
	}

	rnSocketIOServiceTcp* pSession = bnf::instance()->tcp_session_buf_[shandle];
	pSession->Open( shandle, handle_, read_wait_time_sec_, write_wait_time_sec_ );

	acceptor_.async_accept( pSession->Socket(),
							boost::bind( &ListenSessionTcp::HandleAccept, this, pSession, boost::asio::placeholders::error ) );

	LOG_INFO( "bnf - open listen socket. ip: %s, port: %d, fd: %d, handle: %X", host.c_str(), port, acceptor_.native(), GetHandle() );
	return true;
}

void ListenSessionTcp::Close()
{
	io_service_.post( boost::bind( &ListenSessionTcp::HandleClose, this ) );
}

void ListenSessionTcp::HandleAccept( rnSocketIOService* pSessionService, const boost::system::error_code& error )
{
	if( !error )
	{
		rnSocketIOServiceTcp* pSession = (rnSocketIOServiceTcp*)pSessionService;

		pSession->_getIp();
		bnf::instance()->tcp_session_list_.insert(pSession->GetHandle(), pSession);

		tcp_session_queue_.push(pSession);

		bnf::instance()->PutSessionEvent( SessionEvent::ON_ACCEPT, this );

		LOG_INFO( "bnf accepted - ip : %s : unique id : %010d", pSession->ip().c_str(), pSession->getUniqueId());

		session_handle shandle;

		while( 1 )
		{
			if( bnf::instance()->tcp_session_seq_.pop(shandle) == false )
			{
				bnf::instance()->growSessionBuffer();
				LOG_INFO("session buffer growing.");
			}

			pSession = bnf::instance()->tcp_session_buf_[shandle];
			if( ((rnSocketIOServiceTcp*)pSession)->Socket().is_open() == false )
				break;

			LOG_INFO( "bnf - skip rnSocketIOService. handle: %X, fd: %d", pSession->GetHandle(), pSession->Socket().native() );
		}

		pSession->Open( shandle, handle_, read_wait_time_sec_, write_wait_time_sec_ );
		acceptor_.async_accept( pSession->Socket(),
								boost::bind( &ListenSessionTcp::HandleAccept, this, pSession, boost::asio::placeholders::error ) );
	}
	else
	{
		LOG_ERROR( "bnf - HandleAccept error. error[%s] / fd[%d], handle[%X]", error.message().c_str(), acceptor_.native(), GetHandle());
	}
}

void ListenSessionTcp::HandleClose()
{
	boost::system::error_code error;
	acceptor_.close( error );
}

void ListenSessionTcp::closeOnExecOn()
{
#ifndef WIN32
	int oldflags = fcntl (acceptor_.native(), F_GETFD, 0);
	if ( oldflags >= 0 )
	{
		fcntl( acceptor_.native(), F_SETFD, oldflags | FD_CLOEXEC );
	}
#endif
}

void ListenSessionTcp::closeOnExecOff()
{
#ifndef WIN32
	int oldflags = fcntl (acceptor_.native(), F_GETFD, 0);
	if ( oldflags >= 0 )
	{
		fcntl( acceptor_.native(), F_SETFD, oldflags & ~FD_CLOEXEC );
	}
#endif
}
