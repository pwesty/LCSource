#include "timer_session.h"
#include "bnf.h"
#include <ctime>

TimerSession::TimerSession( boost::asio::io_service& io_service )
	: SessionBase(io_service, SessionBase::TIMER_SESSION), timer_( io_service )
{
	running_ = true;

	bnf::instance()->timer_session_seq_.pop( handle_ );
}

TimerSession::~TimerSession()
{
	bnf::instance()->timer_session_seq_.push( handle_ );
}

void TimerSession::Close()
{
	io_service_.post( boost::bind( &TimerSession::Stop, this ) );
}

void TimerSession::Stop()
{
	timer_.cancel();
	running_ = false;
}

//////////////////////////////////////////////////////////////////////////
// MSecTimerSession
MSecTimerSession::MSecTimerSession(boost::asio::io_service& io_service, int start_msec, int period_msec)
	: TimerSession( io_service )
{
	millisecond_ = period_msec;

	timer_.expires_from_now( boost::posix_time::milliseconds( start_msec ) );
	timer_.async_wait( boost::bind( &MSecTimerSession::handle_process, this, _1 ) );

	LOG_INFO( "bnf - create timer. handle: %X, start_msec: %d, period_msec: %d", handle_, start_msec, period_msec );
}

MSecTimerSession::~MSecTimerSession()
{
	LOG_INFO( "bnf - delete timer. handle: %X", handle_ );
}

void MSecTimerSession::handle_process(const boost::system::error_code& error)
{
	if (error || running_ == false)
	{
		delete this;
		return;
	}

	bnf::instance()->PutSessionEvent( SessionEvent::ON_TIMER, this );
	timer_.expires_from_now( boost::posix_time::milliseconds( millisecond_ ) );
	timer_.async_wait( boost::bind( &MSecTimerSession::handle_process, this, _1 ) );
}

//////////////////////////////////////////////////////////////////////////
// SecTimerSession
SecTimerSession::SecTimerSession(boost::asio::io_service& io_service, int start_sec, int period_sec)
	: TimerSession( io_service )
{
	second_ = period_sec;

	timer_.expires_from_now( boost::posix_time::seconds( start_sec ) );
	timer_.async_wait( boost::bind( &SecTimerSession::handle_process, this, _1 ) );

	LOG_INFO( "bnf - create timer. handle: %X, start_sec: %d, period_sec: %d", handle_, start_sec, period_sec );
}

SecTimerSession::~SecTimerSession()
{
	LOG_INFO( "bnf - delete timer. handle: %X", handle_ );
}

void SecTimerSession::handle_process(const boost::system::error_code& error)
{
	if (error || running_ == false)
	{
		delete this;
		return;
	}

	bnf::instance()->PutSessionEvent( SessionEvent::ON_TIMER, this );
	timer_.expires_from_now( boost::posix_time::seconds( second_ ) );
	timer_.async_wait( boost::bind( &SecTimerSession::handle_process, this, _1 ) );
}

//////////////////////////////////////////////////////////////////////////
// AbsoluteTimerSession
AbsoluteTimerSession::AbsoluteTimerSession( boost::asio::io_service& io_service, int start_sec, int period_sec ) :
	TimerSession( io_service )
{
	second_ = period_sec;

	boost::posix_time::ptime now = boost::posix_time::second_clock::universal_time();
	timer_.expires_at( now + boost::posix_time::seconds( start_sec ) );
	timer_.async_wait( boost::bind( &AbsoluteTimerSession::handle_process, this, _1 ) );

	LOG_INFO( "bnf - create timer. handle: %X, start_sec: %d, period_sec: %d", handle_, start_sec, period_sec );
}

AbsoluteTimerSession::~AbsoluteTimerSession()
{
	LOG_INFO( "bnf - delete timer. handle: %X", handle_ );
}

void AbsoluteTimerSession::handle_process( const boost::system::error_code& error )
{
	if (error || running_ == false)
	{
		delete this;
		return;
	}

	bnf::instance()->PutSessionEvent( SessionEvent::ON_TIMER, this );
	timer_.expires_at( timer_.expires_at() + boost::posix_time::seconds( second_ ) );
	timer_.async_wait( boost::bind( &AbsoluteTimerSession::handle_process, this, _1 ) );
}