#include <boost/atomic.hpp>
#include <boost/format.hpp>

#include "bnf.h"
#include "rnsocketioserviceTcp.h"
#include "logsystem.h"
#include "rnpacket.h"

namespace
{
	boost::atomic<int>		local_unique_id(1);
}

rnSocketIOServiceTcp::rnSocketIOServiceTcp(boost::asio::io_service& io_service)
	: rnSocketIOService(io_service, SessionBase::SESSION)
	, socket_(io_service)
	, read_wait_timer_(io_service)
	, read_wait_time_sec_(0)
	, write_wait_timer_(io_service)
	, write_wait_time_sec_(0)
	, ip_("")
	, ipnumber_(0)
	, listen_session_handle_(SessionBase::INVALID_SESSION_HANDLE)
	, now_packet_(NULL)
	, now_packet_for_billing_(NULL)
	, write_error_(false)
	, timeout_(false)
	, close_flag_(false)
	, close_completed_(false)
	, is_call_close_function_(false)
	, seq_(1)
	, check_seq_flag_(false)
	, packet_type_(CONNECT_TYPE_FOR_CNETMSG)
	, boost_strand_(io_service)
{
}

rnSocketIOServiceTcp::~rnSocketIOServiceTcp()
{
	if (close_flag_ == false && socket_.is_open())
	{
		boost::system::error_code error;
		socket_.close( error );
	}

	if( now_packet_ != NULL )
		delete now_packet_;
}

void rnSocketIOServiceTcp::Open(session_handle handle, int packet_type)
{
	Init();

	this->packet_type_ = packet_type;

	SetHandle(handle);
	listen_session_handle_ = SessionBase::INVALID_SESSION_HANDLE;
}

void rnSocketIOServiceTcp::Open(session_handle handle, session_handle listen_handle, int read_wait_time_sec, int write_wait_time_sec)
{
	Init();

	SetHandle(handle);
	listen_session_handle_ = listen_handle;
	read_wait_time_sec_ = read_wait_time_sec;
	write_wait_time_sec_ = write_wait_time_sec;
}

bool rnSocketIOServiceTcp::AsyncConnect( const std::string& host, int port )
{
	std::ostringstream oss;
	oss << port;

	std::string port_string = oss.str();

	boost::asio::ip::tcp::resolver resolver(io_service_);
	boost::asio::ip::tcp::resolver::query query(host, port_string);

	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

	LOG_DEBUG( "bnf async connecting - ip(to) :  %s : port : %d : unique id : %010d", host.c_str(), port, this->getUniqueId());

	socket_.async_connect( endpoint, boost_strand_.wrap(boost::bind( &rnSocketIOServiceTcp::HandleAsyncConnect, this, boost::asio::placeholders::error, ++endpoint_iterator )));
	return true;
}

void rnSocketIOServiceTcp::Run()
{
	NoDelayOn();

	SetReadWaitTimer();

	switch (this->packet_type_)
	{
	case CONNECT_TYPE_FOR_CBILLING:
		boost::asio::async_read(socket_,
								boost::asio::buffer(&packet_header_for_billing_, sizeof(packet_header_for_billing_)),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadHeaderForBilling, this, boost::asio::placeholders::error )));
		break;

	case CONNECT_TYPE_FOR_CNETMSG:
		boost::asio::async_read(socket_,
								boost::asio::buffer(&packet_header_, sizeof(packet_header_)),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadHeader, this, boost::asio::placeholders::error )));
		break;

	case CONNECT_TYPE_FOR_TLD_BILLING:
		boost::asio::async_read(socket_,
								boost::asio::buffer(&packet_header_, sizeof(packet_header_)),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadHeaderForTLDBilling, this, boost::asio::placeholders::error )));
		break;
	}
}

void rnSocketIOServiceTcp::Close( std::string err_str )
{
	if (is_call_close_function_)
		return;

	is_call_close_function_ = true;

	LOG_INFO( "bnf manual close - ip : %s : unique id : %010d : reason : %s",
		ip().c_str(), this->getUniqueId(), err_str.c_str());

	boost_strand_.post( boost::bind( &rnSocketIOServiceTcp::__close, this ) );
}

void rnSocketIOServiceTcp::deliver( CNetMsg::SP packet)
{
	if (close_flag_ || is_call_close_function_ || isInvalidAction())
	{
		LOG_INFO("ip : %s : unique id : %010d : close_flag_ : %d : is_call_close_function_ : %d : socket.is_open : %d : io_service_.stopped : %d",
			this->ip_.c_str(), this->getUniqueId(), close_flag_, is_call_close_function_, this->socket_.is_open(), this->io_service_.stopped());
		return;
	}

	if (packet->make_header_flag_ == false)
	{
		packet->makeHeader();
	}

	if (this->crypt_flag_ == true && packet->crypt_flag_ == false)
	{
		packet->crypt();
	}

	if (packet->crc32_flag_ == false)
	{
		packet->makeCRC32();
	}

	boost_strand_.post(boost::bind( &rnSocketIOServiceTcp::__deliver, this, packet ));
}

void rnSocketIOServiceTcp::deliver( CBPacket::SP packet )
{
	if (close_flag_ || is_call_close_function_ || isInvalidAction())
	{
		LOG_INFO("ip : %s : unique id : %010d : close_flag_ : %d : is_call_close_function_ : %d : socket.is_open : %d : io_service_.stopped : %d",
			this->ip_.c_str(), this->getUniqueId(), close_flag_, is_call_close_function_, this->socket_.is_open(), this->io_service_.stopped());
		return;
	}

	if (packet->make_header_flag_ == false)
	{
		packet->makeHeader();
	}

	boost_strand_.post(boost::bind( &rnSocketIOServiceTcp::__deliver_for_cbpacket, this, packet ));
}

std::string& rnSocketIOServiceTcp::ip()
{
	return ip_;
}

unsigned int rnSocketIOServiceTcp::ipnumber()
{
	return ipnumber_;
}

void rnSocketIOServiceTcp::ShutdownRead()
{
	boost::system::error_code error;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, error);
}

void rnSocketIOServiceTcp::ShutdownWrite()
{
	boost::system::error_code error;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, error);
}

void rnSocketIOServiceTcp::ShutdownBoth()
{
	boost::system::error_code error;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
}

void rnSocketIOServiceTcp::ReceiveBufferSize( size_t size )
{
	boost::asio::socket_base::receive_buffer_size option(size);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

int rnSocketIOServiceTcp::ReceiveBufferSize()
{
	boost::asio::socket_base::receive_buffer_size option;

	boost::system::error_code error;
	socket_.get_option(option, error);
	return option.value();
}

void rnSocketIOServiceTcp::SendBufferSize( size_t size )
{
	boost::asio::socket_base::send_buffer_size option(size);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

int rnSocketIOServiceTcp::SendBufferSize()
{
	boost::asio::socket_base::send_buffer_size option;

	boost::system::error_code error;
	socket_.get_option(option, error);
	return option.value();
}

void rnSocketIOServiceTcp::NoDelayOn()
{
	boost::asio::ip::tcp::no_delay option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::NoDelayOff()
{
	boost::asio::ip::tcp::no_delay option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::DoNotRouteOn()
{
	boost::asio::socket_base::do_not_route option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::DoNotRouteOff()
{
	boost::asio::socket_base::do_not_route option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::Linger( bool onOff, int linger )
{
	boost::asio::socket_base::linger option(onOff, linger);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::DebugOn()
{
	boost::asio::socket_base::debug option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::DebugOff()
{
	boost::asio::socket_base::debug option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::KeepAliveOn()
{
	boost::asio::socket_base::keep_alive option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::KeepAliveOff()
{
	boost::asio::socket_base::keep_alive option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::BroadcastOn()
{
	boost::asio::socket_base::broadcast option(true);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::BroadcastOff()
{
	boost::asio::socket_base::broadcast option(false);

	boost::system::error_code error;
	socket_.set_option(option, error);
}

void rnSocketIOServiceTcp::NonBlockingIoOn()
{
	boost::asio::socket_base::non_blocking_io command(true);

	boost::system::error_code error;
	socket_.io_control(command, error);
}

void rnSocketIOServiceTcp::NonBlockingIoOff()
{
	boost::asio::socket_base::non_blocking_io command(false);

	boost::system::error_code error;
	socket_.io_control(command, error);
}

void rnSocketIOServiceTcp::closeOnExecOn()
{
#ifndef WIN32
	int oldflags = fcntl (socket_.native(), F_GETFD, 0);
	if ( oldflags >= 0 )
	{
		fcntl( socket_.native(), F_SETFD, oldflags | FD_CLOEXEC );
	}
#endif
}

void rnSocketIOServiceTcp::closeOnExecOff()
{
#ifndef WIN32
	int oldflags = fcntl (socket_.native(), F_GETFD, 0);
	if ( oldflags >= 0 )
	{
		fcntl( socket_.native(), F_SETFD, oldflags & ~FD_CLOEXEC );
	}
#endif
}

void rnSocketIOServiceTcp::_getIp()
{
	boost::system::error_code error;

	const boost::asio::ip::tcp::endpoint& remode_endpoint = socket_.remote_endpoint(error);
	if(error)
	{
		LOG_ERROR( "bnf - socket_.remote_endpoint error. error[%s] / fd[%d] / handle[%X]",
				   error.message().c_str(), socket_.native(), GetHandle());
		return;
	}

	ip_ =  remode_endpoint.address().to_string(error);
	ipnumber_ =  remode_endpoint.address().to_v4().to_ulong();
}

void rnSocketIOServiceTcp::Init()
{
	Linger( 0, 0 );

	// SessionBase
	type_ = 0;
	handle_ = 0;
	user_data_ = NULL;
	error_message_.clear();

	// rnSocketIOServiceTcp
	read_wait_time_sec_ = 0;
	write_wait_time_sec_ = 0;

	ip_.clear();
	ipnumber_ = 0;
	close_flag_ = false;
	close_completed_ = false;
	is_call_close_function_ = false;
	listen_session_handle_ = 0;

	if( now_packet_ != NULL )
	{
		delete now_packet_;
		now_packet_ = NULL;
	}

	read_queue_.clear();
	write_queue_.clear();
	read_queue_for_billing_.clear();
	write_queue_for_billing_.clear();

	timeout_ = false;
	close_error_ = false;
	write_error_ = false;
	crypt_flag_ = false;

	seq_ = 1;
	unique_id_ = local_unique_id.fetch_add(1);
}

void rnSocketIOServiceTcp::SetReadWaitTimer()
{
	if (this->io_service_.stopped())
		return;

	if (read_wait_time_sec_ > 0)
	{
		read_wait_timer_.expires_from_now( boost::posix_time::seconds( read_wait_time_sec_ ) );
		read_wait_timer_.async_wait( boost_strand_.wrap(boost::bind( &rnSocketIOServiceTcp::ReadWaitTimerClose, this, _1 )) );
	}
}

void rnSocketIOServiceTcp::SetWriteWaitTimer()
{
	if (this->io_service_.stopped())
		return;

	if (write_wait_time_sec_ > 0)
	{
		write_wait_timer_.expires_from_now( boost::posix_time::seconds( write_wait_time_sec_ ) );
		write_wait_timer_.async_wait( boost_strand_.wrap(boost::bind( &rnSocketIOServiceTcp::WriteWaitTimerClose, this, _1 )) );
	}
}

void rnSocketIOServiceTcp::HandleAsyncConnect( const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator )
{
	if( !error )
	{
		// 성공
		_getIp();
		LOG_INFO( "bnf connected - ip : %s : unique id : %010d", ip().c_str(), getUniqueId());

		bnf::instance()->PutSessionEvent( SessionEvent::ON_CONNECT, this );

		Run();
	}
	else if( endpoint_iterator != boost::asio::ip::tcp::resolver::iterator() )
	{
		// 다른 ip가 있다면 다른 ip로 접속 시도
		boost::system::error_code error;
		socket_.close( error );

		boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;

		socket_.async_connect( endpoint, boost_strand_.wrap(boost::bind( &rnSocketIOServiceTcp::HandleAsyncConnect, this, boost::asio::placeholders::error, ++endpoint_iterator )));
	}
	else
	{
		// 실패
		LOG_ERROR( "bnf connect failed - ip : %s : unique id : %010d : reason : %s",
				   ip().c_str(), getUniqueId(), error.message().c_str());

		bnf::instance()->PutSessionEvent( SessionEvent::ON_CONNECT_FAIL, this );

		__close();
	}
}

void rnSocketIOServiceTcp::HandleReadHeader(const boost::system::error_code& error)
{
	if (error)
	{
		if (close_flag_ == false)
		{
			LOG_INFO( "bnf HandleReadHeader error - ip : %s : unique id : %010d : reason : %s",
					   ip().c_str(), getUniqueId(), error.message().c_str());
		}

		__close();
		return;
	}

	HTONS(packet_header_.reliable);
	HTONL(packet_header_.seq);
	HTONL(packet_header_.size);

	if (this->check_seq_flag_)
	{
		if (this->seq_ != packet_header_.seq)
		{
			LOG_ERROR( "bnf HandleReadHeader error - ip : %s : unique id : %010d : reason : invalid packet sequence[%d] - server sequence[%d]",
				ip().c_str(), getUniqueId(), packet_header_.seq, this->seq_);

			__close();
			return;
		}

		++this->seq_;
	}

	// 클라가 패킷을 잘못 보냈을 경우
	if( packet_header_.size > (MAX_MESSAGE_SIZE - sizeof(MsgHeader) - sizeof(int)))
	{
		LOG_ERROR( "bnf HandleReadHeader error - ip : %s : unique id : %010d : reason : packet size[%d] is too big",
				   ip().c_str(), getUniqueId(), packet_header_.size);

		__close();
		return;
	}

	SetReadWaitTimer();

	//////////////////////////////////////////////////////////////////////////
	now_packet_ = new CNetMsg;
	now_packet_->m_size = packet_header_.size;
	//////////////////////////////////////////////////////////////////////////

	{
		if (close_flag_ == true || isInvalidAction())
			return;

		boost::asio::async_read(socket_,
								boost::asio::buffer(now_packet_->m_buf, packet_header_.size + sizeof(int)),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadBody, this, boost::asio::placeholders::error)));
	}
}

void rnSocketIOServiceTcp::HandleReadBody(const boost::system::error_code& error)
{
	if (error)
	{
		if( now_packet_ != NULL )
		{
			delete now_packet_;
			now_packet_ = NULL;
		}

		if (close_flag_ == false)
		{
			LOG_INFO( "bnf HandleReadBody error - ip : %s : unique id : %010d : reason : %s",
				ip().c_str(), getUniqueId(), error.message().c_str());
		}

		__close();
		return;
	}

	SetReadWaitTimer();

	//////////////////////////////////////////////////////////////////////////
	memcpy((void *)now_packet_->m_buf_all, (void *)&packet_header_, sizeof(packet_header_));
	if (now_packet_->checkCRC32() == false)
	{
		// 암호를 강제로 풀어준뒤 type값을 얻어내어 로그를 출력함
		if (this->crypt_flag_)
		{
			now_packet_->decrypt();
		}

		int type = (int)now_packet_->m_buf[0];
		int subtype = (int)now_packet_->m_buf[1];

		LOG_ERROR( "bnf HandleReadBody error - ip : %s : unique id : %010d : reason : invalid CRC32",
			ip().c_str(), getUniqueId());

		__close();
		return;
	}

	if (this->crypt_flag_)
	{
		now_packet_->decrypt();
	}

	now_packet_->m_mtype = (int)now_packet_->m_buf[0];
// 	now_packet_->crc32_flag_ = true; // 클라이언트(?)로 부터 들어온 모든 패킷은 기본적으로 CRC32값을 가지고 있음

	now_packet_->m_ptr = 1;	// 패킷 바디중 첫번째 바이트값은 m_type으로 가져갔으므로 그 다음을 가르키게 한다.
	//////////////////////////////////////////////////////////////////////////

	read_queue_.push(now_packet_);
	now_packet_ = NULL;

	bnf::instance()->PutSessionEvent( SessionEvent::ON_RECEIVE, this );

	{
		if (close_flag_ == true || isInvalidAction())
			return;

		boost::asio::async_read(socket_,
								boost::asio::buffer(&packet_header_, sizeof(packet_header_)),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadHeader, this, boost::asio::placeholders::error )));
	}
}

void rnSocketIOServiceTcp::HandleWrite( const boost::system::error_code& error, std::size_t bytes_transferred )
{
	if (close_completed_ || isInvalidAction())
		return;

	if (error)
	{
		LOG_INFO( "bnf HandleWrite error - ip : %s : unique id : %010d : reason : %s",
			ip().c_str(), getUniqueId(), error.message().c_str());

		write_queue_.clear();
		__close();
		return;
	}

	{
		write_queue_.pop_front();
		if (write_queue_.empty())
		{
			if (close_flag_)
				__close();

			return;
		}
		
		if( write_wait_time_sec_ > 0 )
			SetWriteWaitTimer();

		CNetMsg* next_packet = write_queue_.front().get();
		int write_size = next_packet->m_size + sizeof(MsgHeader) + sizeof(int)/* crc32 */;
		boost::asio::async_write(socket_,
			boost::asio::buffer((void *)next_packet->m_buf_all, write_size),
			boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleWrite, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
	}
}

void rnSocketIOServiceTcp::ReadWaitTimerClose(const boost::system::error_code& error)
{
	if (error)
		return;

	timeout_ = true;
	LOG_INFO( "bnf read time out - ip : %s : unique id : %010d", ip().c_str(), getUniqueId() );

	write_queue_.clear();
	__close();
}

void rnSocketIOServiceTcp::WriteWaitTimerClose(const boost::system::error_code& error)
{
	if (error)
		return;

	timeout_ = true;
	LOG_INFO( "bnf write time out - ip : %s : unique id : %010d", ip().c_str(), getUniqueId() );

	write_queue_.clear();
	__close();
}

void rnSocketIOServiceTcp::__close()
{
	close_flag_ = true;

	if( read_wait_time_sec_ > 0 )
		read_wait_timer_.cancel();

	if( write_wait_time_sec_ > 0 )
		write_wait_timer_.cancel();

	boost::system::error_code error;
	socket_.shutdown( boost::asio::ip::tcp::socket::shutdown_receive, error );

	{
		if (write_queue_.empty() == false)
			return;

		if (close_completed_ == true || socket_.is_open() == false)
			return;

		close_completed_ = true;

		LOG_INFO( "bnf closed - ip : %s : unique id : %010d",
			ip().c_str(), this->getUniqueId());

		if (user_data_ != NULL)
		{
			read_queue_.push(NULL);
			bnf::instance()->PutSessionEvent( SessionEvent::ON_CLOSE, this );
		}
		else
		{
			bnf::instance()->RemoveSession( this );
		}

		boost::system::error_code error;
		socket_.close( error );
		if( error )
		{
			LOG_ERROR( "bnf socket_.close error - ip : %s : unique id : %010d : reason : %s",
				ip().c_str(), this->getUniqueId(), error.message().c_str() );
			close_error_ = true;
		}
	}
}

// for CBilling
void rnSocketIOServiceTcp::HandleReadHeaderForBilling( const boost::system::error_code& error )
{
	if (error)
	{
		if (close_flag_ == false)
		{
			LOG_INFO( "bnf HandleReadHeader error - ip : %s : unique id : %010d : reason : %s",
				ip().c_str(), getUniqueId(), error.message().c_str());
		}

		__close();
		return;
	}

	NTOHS(packet_header_for_billing_.type);
	NTOHL(packet_header_for_billing_.sn);
	NTOHS(packet_header_for_billing_.size);

	// 클라가 패킷을 잘못 보냈을 경우
	if( packet_header_for_billing_.size > (MAX_PACKET_SIZE - sizeof(BPacketHeader)))
	{
		LOG_ERROR( "bnf HandleReadHeader error - ip : %s : unique id : %010d : reason : packet size[%d] is too big",
			ip().c_str(), getUniqueId(), packet_header_for_billing_.size);

		__close();
		return;
	}

	SetReadWaitTimer();

	//////////////////////////////////////////////////////////////////////////
	now_packet_for_billing_ = new CBPacket;
	//////////////////////////////////////////////////////////////////////////

	{
		if (close_flag_ == true || isInvalidAction())
			return;

		boost::asio::async_read(socket_,
								boost::asio::buffer(now_packet_for_billing_->m_buf, packet_header_for_billing_.size),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadBodyForBilling, this, boost::asio::placeholders::error)));
	}
}

void rnSocketIOServiceTcp::HandleReadBodyForBilling( const boost::system::error_code& error )
{
	if (error)
	{
		if( now_packet_for_billing_ != NULL )
		{
			delete now_packet_for_billing_;
			now_packet_for_billing_ = NULL;
		}

		if (close_flag_ == false)
		{
			LOG_INFO( "bnf HandleReadHeader error - ip : %s : unique id : %010d : reason : %s",
				ip().c_str(), getUniqueId(), error.message().c_str());
		}

		__close();
		return;
	}

	SetReadWaitTimer();

	//////////////////////////////////////////////////////////////////////////
	memcpy((void *)now_packet_for_billing_->m_buf_all, (void *)&packet_header_for_billing_, sizeof(packet_header_for_billing_));
	now_packet_for_billing_->Init(packet_header_for_billing_.type);
	now_packet_for_billing_->m_serial = packet_header_for_billing_.sn;
	now_packet_for_billing_->m_size = packet_header_for_billing_.size;
	//////////////////////////////////////////////////////////////////////////

	read_queue_for_billing_.push(now_packet_for_billing_);
	now_packet_for_billing_ = NULL;

	bnf::instance()->PutSessionEvent( SessionEvent::ON_RECEIVE, this );

	{
		if (close_flag_ == true || isInvalidAction())
			return;

		boost::asio::async_read(socket_,
								boost::asio::buffer(&packet_header_for_billing_, sizeof(packet_header_for_billing_)),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadHeaderForBilling, this, boost::asio::placeholders::error )));
	}
}

void rnSocketIOServiceTcp::HandleWriteForBilling( const boost::system::error_code& error, std::size_t bytes_transferred )
{
	if (close_completed_ || isInvalidAction())
		return;

	if (error)
	{
		LOG_INFO( "bnf HandleWrite error - ip : %s : unique id : %010d : reason : %s",
			ip().c_str(), getUniqueId(), error.message().c_str());

		write_queue_for_billing_.clear();
		__close();
		return;
	}

	{
		write_queue_for_billing_.pop_front();
		if (write_queue_for_billing_.empty())
		{
			if (close_flag_)
				__close();

			return;
		}
		
		if( write_wait_time_sec_ > 0 )
			SetWriteWaitTimer();

		CBPacket* next_packet = write_queue_for_billing_.front().get();
		int write_size = next_packet->m_size + sizeof(BPacketHeader);
		boost::asio::async_write(socket_,
			boost::asio::buffer((void *)next_packet->m_buf_all, write_size),
			boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleWriteForBilling, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
	}
}


//////////////////////////////////////////////////////////////////////////
// for TLD
void rnSocketIOServiceTcp::deliverForTLDBilling( CNetMsg::SP packet)
{
	if (close_flag_ || is_call_close_function_ || isInvalidAction())
	{
		return;
	}

	if (packet->make_header_flag_ == false)
	{
		packet->makeHeader();
	}

	boost_strand_.post(boost::bind( &rnSocketIOServiceTcp::__deliver__for_tld_billing, this, packet ));
}

void rnSocketIOServiceTcp::HandleReadHeaderForTLDBilling(const boost::system::error_code& error)
{
	if (error)
	{
		if (close_flag_ == false)
		{
			LOG_INFO( "bnf HandleReadHeader error - ip : %s : unique id : %010d : reason : %s",
				ip().c_str(), getUniqueId(), error.message().c_str());
		}

		__close();
		return;
	}

	HTONS(packet_header_.reliable);
	HTONL(packet_header_.seq);
	HTONL(packet_header_.size);

	// 클라가 패킷을 잘못 보냈을 경우
	if( packet_header_.size > (MAX_MESSAGE_SIZE - sizeof(MsgHeader)))
	{
		LOG_ERROR( "bnf HandleReadHeader error - ip : %s : unique id : %010d : reason : packet size[%d] is too big",
			ip().c_str(), getUniqueId(), packet_header_.size);

		__close();
		return;
	}

	SetReadWaitTimer();

	//////////////////////////////////////////////////////////////////////////
	now_packet_ = new CNetMsg;
	now_packet_->m_size = packet_header_.size;
	//////////////////////////////////////////////////////////////////////////

	{
		if (close_flag_ == true || isInvalidAction())
			return;

		boost::asio::async_read(socket_,
								boost::asio::buffer(now_packet_->m_buf, packet_header_.size),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadBodyForTLDForBilling, this, boost::asio::placeholders::error)));
	}
}

void rnSocketIOServiceTcp::HandleReadBodyForTLDForBilling(const boost::system::error_code& error)
{
	if (error)
	{
		if( now_packet_ != NULL )
		{
			delete now_packet_;
			now_packet_ = NULL;
		}

		if (close_flag_ == false)
		{
			LOG_INFO( "bnf HandleReadBody error - ip : %s : unique id : %010d : reason : %s",
				ip().c_str(), getUniqueId(), error.message().c_str());
		}

		__close();
		return;
	}

	SetReadWaitTimer();

	//////////////////////////////////////////////////////////////////////////
	memcpy((void *)now_packet_->m_buf_all, (void *)&packet_header_, sizeof(packet_header_));
	now_packet_->m_mtype = (int)now_packet_->m_buf[0];
	// 	now_packet_->crc32_flag_ = true; // 클라이언트(?)로 부터 들어온 모든 패킷은 기본적으로 CRC32값을 가지고 있음

	now_packet_->m_ptr = 1;	// 패킷 바디중 첫번째 바이트값은 m_type으로 가져갔으므로 그 다음을 가르키게 한다.
	//////////////////////////////////////////////////////////////////////////

	read_queue_.push(now_packet_);
	now_packet_ = NULL;

	bnf::instance()->PutSessionEvent( SessionEvent::ON_RECEIVE, this );

	{
		if (close_flag_ == true || isInvalidAction())
			return;

		boost::asio::async_read(socket_,
								boost::asio::buffer(&packet_header_, sizeof(packet_header_)),
								boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleReadHeaderForTLDBilling, this, boost::asio::placeholders::error )));
	}
}

void rnSocketIOServiceTcp::HandleWriteForTLDBilling( const boost::system::error_code& error, std::size_t bytes_transferred )
{
	if (close_completed_ || isInvalidAction())
		return;

	if (error)
	{
		LOG_INFO( "bnf HandleWrite error - ip : %s : unique id : %010d : reason : %s",
			ip().c_str(), getUniqueId(), error.message().c_str());

		write_queue_.clear();
		__close();
		return;
	}

	{
		write_queue_.pop_front();
		if (write_queue_.empty())
		{
			if (close_flag_)
				__close();

			return;
		}
		
		if( write_wait_time_sec_ > 0 )
			SetWriteWaitTimer();

		CNetMsg* next_packet = write_queue_.front().get();
		int write_size = next_packet->m_size + sizeof(MsgHeader);
		boost::asio::async_write(socket_,
			boost::asio::buffer((void *)next_packet->m_buf_all, write_size),
			boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleWriteForTLDBilling, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
	}
}

void rnSocketIOServiceTcp::__deliver( CNetMsg::SP packet )
{
	if( close_flag_ == true || socket_.is_open() == false)
		return;

	write_queue_.push_back(packet);
	size_t write_queue_size = write_queue_.size();
	if (write_queue_size == 1)
	{
		if (write_wait_time_sec_ > 0)
			SetWriteWaitTimer();

		int write_size = packet->m_size + sizeof(MsgHeader) + sizeof(int)/* crc32 */;
		boost::asio::async_write(socket_,
			boost::asio::buffer((void *)packet->m_buf_all, write_size),
			boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleWrite, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
	}
}

void rnSocketIOServiceTcp::__deliver_for_cbpacket( CBPacket::SP packet )
{
	if( close_flag_ == true || socket_.is_open() == false)
		return;

	write_queue_for_billing_.push_back(packet);
	size_t write_queue_size = write_queue_for_billing_.size();
	if (write_queue_size == 1)
	{
		if (write_wait_time_sec_ > 0)
			SetWriteWaitTimer();

		int write_size = packet->m_size + sizeof(BPacketHeader);
		boost::asio::async_write(socket_,
			boost::asio::buffer((void *)packet->m_buf_all, write_size),
			boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleWriteForBilling, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
	}
}

void rnSocketIOServiceTcp::__deliver__for_tld_billing( CNetMsg::SP packet )
{
	if( close_flag_ == true || socket_.is_open() == false)
		return;


	write_queue_.push_back(packet);
	size_t write_queue_size = write_queue_.size();
	if (write_queue_size == 1)
	{
		if (write_wait_time_sec_ > 0)
			SetWriteWaitTimer();
		int write_size = packet->m_size + sizeof(MsgHeader);
		boost::asio::async_write(socket_,
			boost::asio::buffer((void *)packet->m_buf_all, write_size),
			boost_strand_.wrap(boost::bind(&rnSocketIOServiceTcp::HandleWriteForTLDBilling, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
	}
}
