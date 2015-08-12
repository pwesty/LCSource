#ifndef __LISTEN_SESSION_TCP_H__
#define __LISTEN_SESSION_TCP_H__

#include <string>
#include <boost/function.hpp>

#include "MemoryPoolBase.h"
#include "seq_manager.h"
#include "map_ts.h"

#include "listen_session.h"
#include "rnsocketiohandler.h"
#include "rnsocketioserviceTcp.h"

class ListenSessionTcp : public ListenSession, public MemoryPoolBaseWithMutex<ListenSessionTcp>
{
public:
	ListenSessionTcp(boost::asio::io_service& io_service, int read_wait_time_sec = 0, int write_wait_time_sec = 0);
	~ListenSessionTcp();

	virtual bool Run( std::string& host, const int port, rnSocketIOHandler* func,
					  size_t receive_buffer_size = 0, size_t send_buffer_size = 0 );

	virtual void Close();

	virtual void closeOnExecOn();
	virtual void closeOnExecOff();

	rnSocketIOServiceTcp* getTcpSession()		{ return tcp_session_queue_.front_pop(); }

private:
	void HandleAccept( rnSocketIOService* pSession, const boost::system::error_code& error );
	void HandleClose();

private:
	boost::asio::ip::tcp::acceptor acceptor_;

	int read_wait_time_sec_;
	int write_wait_time_sec_;

	queue_ts<rnSocketIOServiceTcp*>		tcp_session_queue_;
};

#endif
