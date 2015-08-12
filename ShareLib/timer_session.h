#ifndef __TIMER_SESSION_H__
#define __TIMER_SESSION_H__

#include "session_base.h"
#include "MemoryPoolBase.h"

class TimerSession : public SessionBase
{
public:
	TimerSession( boost::asio::io_service& io_service );
	~TimerSession();

public:
	void Close();

private:
	void Stop();

protected:
	boost::asio::deadline_timer timer_;
	bool running_;
};

class MSecTimerSession : public TimerSession, public MemoryPoolBaseWithMutex<MSecTimerSession>
{
public:
	MSecTimerSession( boost::asio::io_service& io_service, int start_msec, int period_msec );
	~MSecTimerSession();

private:
	void handle_process( const boost::system::error_code& error );

private:
	int millisecond_;
};

class SecTimerSession : public TimerSession, public MemoryPoolBaseWithMutex<SecTimerSession>
{
public:
	SecTimerSession( boost::asio::io_service& io_service, int start_sec, int period_sec );
	~SecTimerSession();

private:
	void handle_process( const boost::system::error_code& error );

private:
	int second_;
};

class AbsoluteTimerSession : public TimerSession, public MemoryPoolBaseWithMutex<AbsoluteTimerSession>
{
public:
	AbsoluteTimerSession( boost::asio::io_service& io_service, int start_sec, int period_sec );
	~AbsoluteTimerSession();

private:
	void handle_process( const boost::system::error_code& error );

private:
	int second_;
};

#endif
