#ifndef __BNF_H__
#define __BNF_H__

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "map_ts.h"
#include "queue_ts.h"
#include "seq_manager.h"

#include "session_base.h"
#include "listen_sessionTcp.h"
#include "rnsocketioserviceTcp.h"
#include "timer_session.h"
#include "event_session.h"

#define DEFAULT_IO_WORKER_THREAD_COUNT 4

#define SESSION_HANDLE_CHECKSUM				(0xFF000000)
#define SESSION_INDENTIFY					(0x00000000)
#define CONNECT_SESSION_INDENTIFY			(0x01000000)
#define TIMER_SESSION_INDENTIFY				(0x02000000)
#define LISTEN_SESSION_INDENTIFY			(0x04000000)
#define EVENT_SESSION_INDENTIFY				(0x08000000)

class SessionEvent
{
public:
	enum
	{
		ON_EXIT,
		ON_ACCEPT,
		ON_RECEIVE,
		ON_TIMER,
		ON_CONNECT,
		ON_CONNECT_FAIL,
		ON_CLOSE,
		ON_EVENT,
	};

	SessionEvent( tINT type, SessionBase* session ) : type_( type ), session_( session ) {}

	tINT type_;
	SessionBase* session_;
};

enum
{
	CONNECT_TYPE_FOR_CBILLING,
	CONNECT_TYPE_FOR_CNETMSG,
	CONNECT_TYPE_FOR_TLD_BILLING,
};

class bnf
{
public:
	friend class SessionBase;
	friend class ListenSessionTcp;
	friend class rnSocketIOServiceTcp;
	friend class TimerSession;
	friend class EventSession;

public:
	typedef map_ts<session_handle, SessionBase*> map_session_list_t;
	typedef queue_ts<SessionEvent> session_queue_t;

	typedef boost::function<void ()>	session_process_func;

public:
	bnf();
	~bnf();

	static bnf*					instance();

	boost::asio::io_service&	GetIoService()
	{
		return io_service_;
	}

	session_handle				CreateListen(std::string host, int port, int waittimeout,
			rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);
	session_handle				CreateListen(const char* host, int port, int waittimeout,
			rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);

	session_handle				CreateListen_SetWaitTime(std::string host, int port, int read_wait_time_sec, int write_wait_time_sec,
			rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);
	session_handle				CreateListen_SetWaitTime(const char* host, int port, int read_wait_time_sec, int write_wait_time_sec,
			rnSocketIOHandler* func, size_t receive_buffer_size = 0, size_t send_buffer_size = 0);

	void						CloseListen(session_handle handle);

	session_handle				CreateConnect(std::string& host, std::string& port, void* user_data);
	session_handle				CreateConnect(std::string host, int port, void* user_data);
	session_handle 				CreateAsyncConnect( std::string& host, int port, void* user_data, int packet_type = CONNECT_TYPE_FOR_CNETMSG);

	session_handle				CreateMSecTimer(int msec, void *user_data);
	session_handle				CreateMSecTimerPeriod(int start_msec, int period_msec, void *user_data);
	session_handle				CreateSecTimerPeriod(int start_sec, int period_sec, void *user_data);
	session_handle				CreateAbsoluteTimerPeriod( int start_sec, int period_sec, void *user_data );

	session_handle				CreateEvent(void* user_data);

	void						RemoveSession(SessionBase* pBase);

	void						Run( int worker_thread_count = DEFAULT_IO_WORKER_THREAD_COUNT );
	void						Stop();

	size_t						GetSessionCountTcp()
	{
		return tcp_session_list_.size();
	}

	SessionEvent				GetSessionFromQueue()
	{
		return session_queue_.wait_front_pop();
	}
	void						PutSessionEvent( tINT type, SessionBase* session )
	{
		session_queue_.push_signal( SessionEvent( type, session ) );
	}

	tINT						GetSessionQueueCount()
	{
		return session_queue_.size();
	}

	SessionBase*				GetSessionPointer(session_handle handle);

	void						SetSessionProcessFunc( session_process_func func )
	{
		session_process_func_ = func;
	}

	void						Clear();

	void						growSessionBuffer();
	void						growConnectSessionBuffer();

	bool						isRun()
	{
		return run_flag_;
	}

	std::vector<std::string>	getMyIp();

private:
	void						WorkerThread();
	void						RemoveSessionThread();
	void						SessionProcessThread();

private:
	std::vector<rnSocketIOServiceTcp*>	tcp_session_buf_;
	std::vector<rnSocketIOServiceTcp*>	tcp_connect_session_buf_;

	seq_manager_ts<session_handle>		timer_session_seq_;
	seq_manager_ts<session_handle>		tcp_session_seq_;
	seq_manager_ts<session_handle>		tcp_connect_session_seq_;
	seq_manager_ts<session_handle>		tcp_listen_session_seq_;
	seq_manager_ts<session_handle>		event_session_seq_;

	map_session_list_t					timer_session_list_;
	map_session_list_t					tcp_session_list_;
	map_session_list_t					tcp_connect_session_list_;
	map_session_list_t					tcp_listen_session_list_;
	map_session_list_t					event_session_list_;

	session_queue_t						session_queue_;

	boost::thread_group					thread_group_;
	boost::asio::io_service				io_service_;

	queue_ts<session_handle>			remove_session_queue_;

	std::auto_ptr<boost::asio::io_service::work>	io_service_work_;

	bool								stopped_;
	bool								run_flag_;

	session_process_func				session_process_func_;
};

#endif
